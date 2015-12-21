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
    _fields[0].field = MINUTE;
    _fields[1].field = HOUR;
    _fields[2].field = DOM;
    _fields[3].field = MONTH;
    _fields[4].field = DOW;
    _fields[5].field = YEAR;
}

/******************************************************************************/

QString
QCron::
_validCharacters(EField field)
{
    Q_UNUSED(field);
    return "0123456789*-/,";
}

/******************************************************************************/


QCron::IntNode *
QCron::
QCronField::parseInt(QString & str)
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
    switch (field)
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

QCron::RangeNode*
QCron::
QCronField::parseRange(QString & str)
{
    //   qDebug() << "Parsing a Range";
    if (last_node == NULL)
    {
        throw 42;
    }
    RangeNode * range = new RangeNode();
    IntNode * begin = dynamic_cast<IntNode *>(last_node);
    if (begin == NULL)
    {
        throw 42;
    }
    range->begin = begin;
    str.remove(0, 1);
    IntNode * end = dynamic_cast<IntNode *>(parseInt(str));
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

QCron::EveryNode*
QCron::
QCronField::parseEvery(QString & str)
{
//    qDebug() << "Parsing an Every";
    EveryNode * every = new EveryNode();
    every->what = last_node;
    str.remove(0, 1);
    every->freq = parseInt(str);
    return every;
}

QCron::ListNode*
QCron::
QCronField::parseList(QString & str)
{
//    qDebug() << "Parsing a List";
    ListNode * list = new ListNode();
    list->nodes << last_node;
    last_node = list;
    while (str[0] == ',')
    {
        str.remove(0, 1);
        Node * node = parseNode(str);
        list->nodes << node;
        last_node = node;
    }
    return list;
}

QCron::Node *
QCron::
QCronField::
parseNode(QString & str)
{
    //qDebug() << "Parsing a node";
    QChar c = str[0];
    if (c.isDigit())
    {
        return parseInt(str);
    }
    else if ("-" == c)
    {
        return parseRange(str);
    }
    else if ("/" == c)
    {
        return parseEvery(str);
    }
    else if ("*" == c)
    {
        return new AllNode;
    }
    else if ("," == c)
    {
        return parseList(str);
    }
    throw 42;
}

/******************************************************************************/


void
QCron::
QCronField::
parse(QString & str)
{
    try
    {
        last_node = NULL;
        Node * root = parseNode(str);
        if (!str.isEmpty())
        {
            last_node = root;
            root = parseNode(str);
        }
        is_valid = true;
    }
    catch (int)
    {
        is_valid = false;
    }
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
        _is_valid &= _fields[i].is_valid;
    }
}

/******************************************************************************/

/*


 */
