#include "qcronfield.hpp"

#include <QDebug>

/******************************************************************************/

QCronField::
QCronField()
    : _is_valid(false)
{}

/******************************************************************************/

QCronIntNode *
QCronField::
_parseInt(QString & str)
{
    int value = 0;
    int char_idx = 0;
    QChar c = str[char_idx];
    while (c.isDigit())
    {
        value = value * 10 + c.digitValue();
        c = str[++char_idx];
    }
    if (char_idx == 0)
    {
        throw QCronFieldException(QString("%1 contains an invalid integer")
                                  .arg(str));
    }
    str.remove(0, char_idx);
    //qDebug() << "Parsing int" << value;
    if (value < _min || _max < value)
    {
        throw QCronFieldException(QString("Value %1 out of range [%2;%3]")
                                  .arg(value).arg(_min).arg(_max));
    }
    return new QCronIntNode(value);
}

/******************************************************************************/

QCronRangeNode*
QCronField::
_parseRange(QString & str)
{
    //   qDebug() << "Parsing a Range";
    if (_last_node == NULL)
    {
        throw QCronFieldException(QString("Syntax error at %1: range has no beginning")
                                  .arg(str));
    }
    QCronIntNode * begin = dynamic_cast<QCronIntNode *>(_last_node);
    if (begin == NULL)
    {
        throw QCronFieldException(QString("Syntax error: expected an interger at %1")
                                  .arg(str));
    }
    str.remove(0, 1);
    QCronIntNode * end = dynamic_cast<QCronIntNode *>(_parseInt(str));
    if (end == NULL)
    {
        throw QCronFieldException(QString("Syntax error: expected an interger at %1")
                                  .arg(str));
    }
    if (begin->value() > end->value())
    {
        throw QCronFieldException(QString("Invalid range: end is before start at %1")
                                  .arg(str));
    }
    return new QCronRangeNode(begin, end);
}

/******************************************************************************/

QCronEveryNode*
QCronField::
_parseEvery(QString & str)
{
//    qDebug() << "Parsing an Every";
    str.remove(0, 1);
    return new QCronEveryNode(_last_node, _parseInt(str));
}

/******************************************************************************/

QCronListNode*
QCronField::
_parseList(QString & str)
{
//    qDebug() << "Parsing a List";
    QCronListNode * list = new QCronListNode();
    list->nodes() << _last_node;
    _last_node = list;
    while (str[0] == ',')
    {
        str.remove(0, 1);
        QCronNode * node = _parseNode(str);
        list->nodes() << node;
        _last_node = node;
    }
    return list;
}

/******************************************************************************/

QCronNode *
QCronField::
_parseNode(QString & str)
{
    //qDebug() << "Parsing a node";
    QChar c = str[0];
    if (c.isDigit())
    {
        return _parseInt(str);
    }
    else if ("-" == c)
    {
        return _parseRange(str);
    }
    else if ("/" == c)
    {
        return _parseEvery(str);
    }
    else if ("*" == c)
    {
        return new QCronAllNode;
    }
    else if ("," == c)
    {
        return _parseList(str);
    }
    throw QCronFieldException(QString("Unexpected character %1").arg(c));
}

/******************************************************************************/

void
QCronField::
parse(QString & str)
{
    try
    {
        _last_node = NULL;
        _root = _parseNode(str);
        if (!str.isEmpty())
        {
            _last_node = _root;
            _root = _parseNode(str);
        }
        _is_valid = true;
    }
    catch (int)
    {
        _is_valid = false;
    }
}

/******************************************************************************/

int
QCronField::
getDateTimeSection(QDateTime & dt) const
{
    switch (_field)
    {
    case MINUTE: return dt.time().minute();
    case HOUR:   return dt.time().hour();
    case DOM:    return dt.date().day();
    case MONTH:  return dt.date().month();
    case DOW:    return dt.date().dayOfWeek();
    case YEAR:   return dt.date().year();
    default:     qFatal("Shouldn't be here");
    }
}

/******************************************************************************/

void
QCronField::
applyOffset(QDateTime & dt, int offset) const
{
    bool overflow = offset < 0 || (offset == 0 && _field == MINUTE);

    offset += overflow ? 1 : 0;

    switch (_field)
    {
    case MINUTE:
    {
        offset -= overflow ? 1 : 0;
        offset += offset <= 0 ? 60 : 0;
        dt = dt.addSecs(60 * offset);
        break;
    }
    case HOUR:
        {
            dt.addSecs(3600 * offset);
            break;
        }
    case DOM:
    case DOW:
        {
            dt.addDays(offset);
            break;
        }
    case MONTH:
        {
            dt.addMonths(offset);
            break;
        }
    case YEAR:
        {
            dt.addYears(offset);
            break;
        }
    default:
        {
            qFatal("Shouldn't be here");
        }
    }
}

/******************************************************************************/

void
QCronField::
next(QDateTime & dt)
{
    int time_section = getDateTimeSection(dt);
    int offset = _root->next(time_section);
    applyOffset(dt, offset);
}
