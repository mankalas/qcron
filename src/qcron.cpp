#include "qcron.hpp"

#include <cmath>

#include <QDebug>

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
_parsePattern(QString & pattern)
{
    if (pattern.contains("\n"))
    {
        _is_valid = false;
        return;
    }
    QStringList fields = pattern.simplified().split(" ", QString::SkipEmptyParts);
    if (fields.size() != 6)
    {
        _is_valid = false;
        return;
    }
    for (int i = 0; i < 6; ++i)
    {
        _fields[i].parse(fields[i]);
        _is_valid &= _fields[i].isValid();
    }
}

/******************************************************************************/

QDateTime
QCron::
next(int n) const
{
    Q_UNUSED(n);
    //QCronField seconds = _fields[0];
    return QDateTime::currentDateTime();
}
