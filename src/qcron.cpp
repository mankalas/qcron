#include "qcron.hpp"
#include "qcronnode.hpp"

#include <cmath>

#include <QDateTime>
#include <QDebug>
#include <QTimer>

/******************************************************************************/

QCron::
QCron()
{
    _init();
}

/******************************************************************************/

QCron::
QCron(const QString & pattern)
{
    _init();
    _parsePattern(pattern);
    if (_is_valid) {
        _checkState();
    }
}

/******************************************************************************/

QCron::
~QCron()
{
}

/******************************************************************************/

void
QCron::
_init()
{
    _is_valid = true;
    _is_active = false;
    _fields[0].setField(MINUTE);
    _fields[1].setField(HOUR);
    _fields[2].setField(DOM);
    _fields[3].setField(MONTH);
    _fields[4].setField(DOW);
    _fields[5].setField(YEAR);
}

/******************************************************************************/

void
QCron::
_checkState()
{
    int interval_ms = 0;
    if (match(QDateTime::currentDateTime()))
    {
        emit activated();
        _is_active = true;
        interval_ms = 1000 * 60; // one minute
    }
    else
    {
        if (_is_active)
        {
            emit deactivated();
            _is_active = false;
        }
        interval_ms = QDateTime::currentDateTime().secsTo(next()) * 1000;
    }
    QTimer::singleShot(interval_ms,
                       Qt::VeryCoarseTimer,
                       this,
                       SLOT(_checkState()));
}

/******************************************************************************/

void
QCron::
_setError(const QString & error)
{
    _is_valid = false;
    _error = error;
}

/******************************************************************************/

void
QCron::
_parsePattern(const QString & pattern)
{
    if (pattern.contains("\n"))
    {
        _setError("'\n' is an invalid field separator.");
        return;
    }
    QStringList fields = pattern.simplified().split(" ", QString::SkipEmptyParts);
    int nb_fields = fields.size();
    if (nb_fields != 6)
    {
        _setError(QString("Wrong number of fields: expected 6, got %1")
                  .arg(nb_fields));
        return;
    }
    try
    {
        for (int i = 0; i < 6; ++i)
        {
            _fields[i].parse(fields[i]);
            _is_valid &= _fields[i].isValid();
        }
    }
    catch (QCronFieldException & e)
    {
        _setError(e.msg());
    }
}

/******************************************************************************/

QList<EField>
getPreviousFields(EField field)
{
    QList<EField> fields;

    switch (field)
    {
        case YEAR:
            fields << MONTH;
        case MONTH:
            fields << DOM;
        case DOW:
        case DOM:
            fields << HOUR;
        case HOUR:
            fields << MINUTE;
        case MINUTE:
            break;
        default:
            qFatal("Should not be in getPreviousTimeUnit");
    }

    return fields;
}

/******************************************************************************/

void
QCron::
add(QDateTime & dt, EField field, int value)
{
    switch (field)
    {
        case YEAR:
            dt = dt.addYears(value);
            break;
        case MONTH:
            dt = dt.addMonths(value);
            break;
        case DOW:
        case DOM:
            dt = dt.addDays(value);
            break;
        case HOUR:
            dt = dt.addSecs(3600 * value);
            break;
        case MINUTE:
            dt = dt.addSecs(60 * value);
            break;
        default:
            qFatal("Unknown value in add");
    }
    QList<EField> previous_fields = getPreviousFields(field);
    foreach (EField field, previous_fields)
    {
        _fields[field].reset(dt);
    }
}

/******************************************************************************/

void
set(QDateTime & dt, EField field, int value)
{
    QDate date = dt.date();
    QTime time = dt.time();

    switch (field)
    {
        case YEAR:
            dt.setDate(QDate(value, date.month(), date.day()));
            break;
        case MONTH:
            dt.setDate(QDate(date.year(), value, date.day()));
            break;
        case DOW:
        case DOM:
            dt.setDate(QDate(date.year(), date.month(), value));
            break;
        case HOUR:
            dt.setTime(QTime(value, time.minute(), 0));
            break;
        case MINUTE:
            dt.setTime(QTime(time.hour(), value, 0));
            break;
        default:
            qFatal("Unknown value in add");
    }
}

/******************************************************************************/

void
QCron::
catchUp(QDateTime & dt, EField field, int value)
{
    int current_time_unit = _fields[field].getDateTimeSection(dt);
    if (current_time_unit < value)
    {
        add(dt, field, value - current_time_unit);
    }
    else if (current_time_unit == value)
    {
        // Nothing
    }
    else if (current_time_unit > value)
    {
        if (YEAR == field)
        {
            dt = QDateTime();
        }
        else if (MINUTE != field)
        {
            int max = _fields[field].getMax();
            add(dt, field, (max - current_time_unit + value));
        }
    }
}

/******************************************************************************/

void
QCron::
_process(QDateTime & dt, EField field)
{
    QCronNode * node = _fields[field].getRoot();
    if (NULL == node)
    {
        qFatal("Problem");
    }
    node->process(this, dt, field);
}

/******************************************************************************/

QDateTime
QCron::
next()
{
    return next(QDateTime::currentDateTime());
}

/******************************************************************************/

QDateTime
QCron::
next(QDateTime dt)
{
    dt = dt.addSecs(60);
    while (!match(dt))
    {
        //qDebug() << dt << "doesn't match";
        for (int i = YEAR; i >= 0; --i)
        {
            _process(dt, (EField)i);
            if (!dt.isValid())
            {
                return dt;
            }
            while (!_fields[(EField)i].match(dt))
            {
                //qDebug() << dt << (EField)i << "doesn't match";
                int dummy = 1;
                _fields[(EField)i].applyOffset(dt, dummy);
            }
        }
    }
    return dt;
}

/******************************************************************************/

bool
QCron::
match(const QDateTime & dt) const
{
    bool does_match = true;
    for (int i = 0; i < 6; ++i)
    {
        does_match &= _fields[i].match(dt);
    }
    return does_match;
}

/******************************************************************************/
