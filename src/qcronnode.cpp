#include "qcronnode.hpp"
#include "qcronfield.hpp"
#include "qcron.hpp"
#include "holiday.hpp"

#include <QDebug>

/******************************************************************************/

QCronNode::
~QCronNode()
{
}

/******************************************************************************/

void
QCronNode::
setField(QCronField * field)
{
    _field = field;
}

/******************************************************************************/
/******************************************************************************/

QCronIntNode::
QCronIntNode(int v)
    : _value(v)
{
}

/******************************************************************************/

int
QCronIntNode::
value() const
{
    return _value;
}

/******************************************************************************/

int
QCronIntNode::
next(int t) const
{
    return _value - t;
}

/******************************************************************************/

void
QCronIntNode::
process(QCron * cron,
        QDateTime & dt,
        EField field)
{
    cron->catchUp(dt, field, _value);
}

/******************************************************************************/

bool
QCronIntNode::
match(const QDateTime & dt) const
{
    return _field->getDateTimeSection(dt) == _value;
}

/******************************************************************************/
/******************************************************************************/

int
QCronStrNode::
next(int t) const
{
    return t - 1;
}

/******************************************************************************/

void
QCronStrNode::
process(QCron *,
        QDateTime &,
        EField )
{
    // TODO
}

/******************************************************************************/

bool
QCronStrNode::
match(const QDateTime & dt) const
{
    return _field->getDateTimeSection(dt);
}

/******************************************************************************/
/******************************************************************************/

int
QCronAllNode::
next(int t) const
{
    Q_UNUSED(t);
    return 0;
}

/******************************************************************************/

void
QCronAllNode::
process(QCron * cron,
        QDateTime & dt,
        EField field)
{
    Q_UNUSED(cron);
    Q_UNUSED(dt);
    Q_UNUSED(field);
}

/******************************************************************************/

bool
QCronAllNode::
match(const QDateTime & dt) const
{
    Q_UNUSED(dt);
    return true;
}

/******************************************************************************/
/******************************************************************************/

QCronRangeNode::
QCronRangeNode(const QCronIntNode * begin,
               const QCronIntNode * end)
    : _begin(begin)
    , _end(end)
{
}

/******************************************************************************/

void
QCronRangeNode::
process(QCron * cron,
        QDateTime & dt,
        EField field)
{
    int begin = beginValue();
    int end = endValue();
    int current_time_unit = _field->getDateTimeSection(dt);
    if (current_time_unit < begin || end < current_time_unit)
    {
        cron->catchUp(dt, field, begin);
    }
    else
    {
        if (field == MINUTE)
        {
            cron->add(dt, field, 1);
        }
    }
}

/******************************************************************************/

int
QCronRangeNode::
beginValue() const
{
    return _begin->value();
}

/******************************************************************************/

int
QCronRangeNode::
endValue() const
{
    return _end->value();
}

/******************************************************************************/

int
QCronRangeNode::
next(int t) const
{
    if (_begin->value() <= t && t <= _end->value())
    {
        return 1;
    }
    return _begin->value() - t;
}

/******************************************************************************/

bool
QCronRangeNode::
match(const QDateTime & dt) const
{
    int tu = _field->getDateTimeSection(dt);
    return _begin->value() <= tu && tu <= _end->value();
}

/******************************************************************************/
/******************************************************************************/

QCronEveryNode::
QCronEveryNode(QCronNode * what,
               QCronIntNode * freq)
    : _what(what)
    , _freq(freq)
{
}

/******************************************************************************/

void
QCronEveryNode::
process(QCron * cron ,
        QDateTime & dt,
        EField field)
{
    int freq = _freq->value();
    _what->process(cron, dt, field);
    while (_field->getDateTimeSection(dt) % freq)
    {
        cron->add(dt, field, 1);
        _what->process(cron, dt, field);
    }
}


/******************************************************************************/

int
QCronEveryNode::
next(int t) const
{
    int what_next = _what->next(t);
    if (what_next == 1)
    {
        /* what_next == 1 if we're in the right range AND if we're one
         * time unit ahead of it */
        int next = t % _freq->value();
        return _freq->value() - next;
    }
    return what_next;
}

/******************************************************************************/

bool
QCronEveryNode::
match(const QDateTime & dt) const
{
    int tu = _field->getDateTimeSection(dt);
    return _what->match(dt) && tu % _freq->value() == 0;
}

/******************************************************************************/
/******************************************************************************/

QList<QCronNode *> &
QCronListNode::
nodes()
{
    return _nodes;
}

/******************************************************************************/

void
QCronListNode::
process(QCron * cron,
        QDateTime & dt,
        EField field)
{
    foreach (QCronNode * node, _nodes)
    {
        int node_next = node->next(_field->getDateTimeSection(dt));
        if (node_next >= 0)
        {
            node->process(cron, dt, field);
            return;
        }
    }
    _nodes[0]->process(cron, dt, field);
}

/******************************************************************************/

int
QCronListNode::
next(int t) const
{
    foreach (const QCronNode * node, _nodes)
    {
        int node_next = node->next(t);
        if (node_next > 0 && node_next != _field->getMax() + 1)
        {
            return node_next;
        }
    }
    return _nodes[0]->next(t);
}

/******************************************************************************/

bool
QCronListNode::
match(const QDateTime & dt) const
{
    foreach (const QCronNode * node, _nodes)
    {
        if (node->match(dt))
        {
            return true;
        }
    }
    return false;
}

/******************************************************************************/
/******************************************************************************/

int
QCronHolidayNode::
next(int t) const
{
    Q_UNUSED(t)
    return -1;
}

/******************************************************************************/

void
QCronHolidayNode::
process(QCron * cron,
        QDateTime & dt,
        EField field)
{
    Q_UNUSED(cron);
    if (DOM != field)
    {
        qFatal("Should not be in a Holiday node.");
    }
    if (!Holiday::isHoliday(dt.date()))
    {
        QDate next_holiday = Holiday::next(dt.date());
        if (dt.date() != next_holiday)
        {
            dt.setDate(next_holiday);
            dt.setTime(QTime(0, 0, 0));
        }
    }
}

/******************************************************************************/

bool
QCronHolidayNode::
match(const QDateTime & dt) const
{
    return Holiday::isHoliday(dt.date());
}

/******************************************************************************/
/******************************************************************************/

int
QCronNotHolidayNode::
next(int t) const
{
    Q_UNUSED(t)
    return -1;
}

/******************************************************************************/

void
QCronNotHolidayNode::
process(QCron * cron,
        QDateTime & dt,
        EField field)
{
    Q_UNUSED(cron);
    if (DOM != field)
    {
        qFatal("Should not be in a NotHoliday node.");
    }
    if (Holiday::isHoliday(dt.date()))
    {
        dt = dt.addDays(1);
        dt.setTime(QTime(0, 0, 0));
    }
}

/******************************************************************************/

bool
QCronNotHolidayNode::
match(const QDateTime & dt) const
{
    return !Holiday::isHoliday(dt.date());
}

/******************************************************************************/
/******************************************************************************/
