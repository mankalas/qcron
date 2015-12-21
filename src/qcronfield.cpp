#include "qcronfield.hpp"

/******************************************************************************/

QCronField::
QCronField()
    : _is_valid(false)
{}

/******************************************************************************/

IntNode *
QCronField::
_parseInt(QString & str)
{
    IntNode * node = new IntNode();
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
        throw 42;
    }
    str.remove(0, char_idx);
    node->value = value;
    //qDebug() << "Parsing an Int :" << value;
    switch (_field)
    {
    case MINUTE: if (value < 0 || 59 < value) throw 42; break;
    case HOUR:   if (value < 0 || 23 < value) throw 42; break;
    case DOM:    if (value < 1 || 31 < value) throw 42; break;
    case MONTH:  if (value < 1 || 12 < value) throw 42; break;
    case DOW:    if (value < 1 || 7  < value) throw 42; break;
    case YEAR:   if (value < 2016 || 2099 < value) throw 42; break;
    default:                                  throw 42; break;
    }
    return node;
}

/******************************************************************************/

RangeNode*
QCronField::
_parseRange(QString & str)
{
    //   qDebug() << "Parsing a Range";
    if (_last_node == NULL)
    {
        throw 42;
    }
    RangeNode * range = new RangeNode();
    IntNode * begin = dynamic_cast<IntNode *>(_last_node);
    if (begin == NULL)
    {
        throw 42;
    }
    range->begin = begin;
    str.remove(0, 1);
    IntNode * end = dynamic_cast<IntNode *>(_parseInt(str));
    if (end == NULL)
    {
        throw 42;
    }
    range->end = end;
    if (range->begin->value > range->end->value)
    {
        throw 42;
    }
    return range;
}

/******************************************************************************/

EveryNode*
QCronField::
_parseEvery(QString & str)
{
//    qDebug() << "Parsing an Every";
    EveryNode * every = new EveryNode();
    every->what = _last_node;
    str.remove(0, 1);
    every->freq = _parseInt(str);
    return every;
}

/******************************************************************************/

ListNode*
QCronField::
_parseList(QString & str)
{
//    qDebug() << "Parsing a List";
    ListNode * list = new ListNode();
    list->nodes << _last_node;
    _last_node = list;
    while (str[0] == ',')
    {
        str.remove(0, 1);
        Node * node = _parseNode(str);
        list->nodes << node;
        _last_node = node;
    }
    return list;
}

/******************************************************************************/

Node *
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
        return new AllNode;
    }
    else if ("," == c)
    {
        return _parseList(str);
    }
    throw 42;
}

/******************************************************************************/

void
QCronField::
parse(QString & str)
{
    try
    {
        _last_node = NULL;
        Node * _root = _parseNode(str);
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
