#include "qcronnode.hpp"

/******************************************************************************/

QCronNode::
~QCronNode()
{
}

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

int
QCronStrNode::
next(int t) const
        {
            return t - 1;
        }

/******************************************************************************/

int
QCronAllNode::
next(int t) const
{
    Q_UNUSED(t);
    return 1;
}

/******************************************************************************/

QCronRangeNode::
QCronRangeNode(const QCronIntNode * begin,
               const QCronIntNode * end)
    : _begin(begin)
    , _end(end)
{
}

/******************************************************************************/

int
QCronRangeNode::
next(int t) const
{
    if (_begin->value() <= t && t < _end->value())
    {
        return 0;
    }
    return _begin->value() - t;
}

/******************************************************************************/

QCronEveryNode::
QCronEveryNode(QCronNode * what,
               QCronIntNode * freq)
    : _what(what)
    , _freq(freq)
{
}

/******************************************************************************/

int
QCronEveryNode::
next(int t) const
{
    int next = _what->next(t) % _freq->value();
    return _freq->value() - next;
}

/******************************************************************************/

QList<QCronNode *>
QCronListNode::
nodes() const
{
    return _nodes;
}

/******************************************************************************/

int
QCronListNode::
next(int t) const
{
    foreach (const QCronNode * node, _nodes)
    {
        int node_next = node->next(t);
        if (t > 0)
        {
            return node_next;
        }
    }
    return _nodes[0]->next(t);
}

/******************************************************************************/
