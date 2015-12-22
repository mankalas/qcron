#include "qcron.hpp"

#include <cmath>

#include <QDebug>
#include <QTime>

/******************************************************************************/

QCron::
QCron()
{
    _init();
}

/******************************************************************************/

QCron::
QCron(QString & pattern)
{
    _init();
    _parsePattern(pattern);
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
_setError(const QString & error)
{
    _is_valid = false;
    _error = error;
}

/******************************************************************************/

void
QCron::
_parsePattern(QString & pattern)
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

QDateTime
QCron::
next(QDateTime dt)
{
    for (int i = 0; i < 6; ++i)
    {
        _fields[i].next(dt);
    }
    return dt;
}

/******************************************************************************/

QDateTime
QCron::
next(int n)
{
    Q_UNUSED(n);
    return next(_beginning);
}

/******************************************************************************/
