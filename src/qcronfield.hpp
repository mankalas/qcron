#ifndef _QCRONFIELD_HPP
#define _QCRONFIELD_HPP

#include <QList>
#include <QString>

enum EField
{
    MINUTE,
    HOUR,
    DOM,
    MONTH,
    DOW,
    YEAR
};

struct Node
{
    virtual ~Node() {}
};

struct ValueNode : public Node
{
};

struct IntNode : public ValueNode
{
    int value;
};

struct StrNode : public ValueNode
{
};

struct AllNode : public ValueNode
{
};

struct RangeNode : public Node
{
    IntNode * begin;
    IntNode * end;
};

struct EveryNode : public Node
{
    Node * what;
    IntNode * freq;
};

struct ListNode : public Node
{
    QList<Node*> nodes;
};

class QCronField
{
public:
    QCronField();

    // Accessors.
    void setField(EField field)
        { _field = field; }

    // Features.
    void parse(QString & str);

    bool isValid() const
        { return _is_valid; }

private:
    EField _field;
    bool _is_valid;
    Node * _last_node;
    Node * _root;

    IntNode * _parseInt(QString & str);
    RangeNode* _parseRange(QString & str);
    EveryNode* _parseEvery(QString & str);
    ListNode * _parseList(QString & str);
    Node * _parseNode(QString & str);
};

#endif
