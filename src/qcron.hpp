#ifndef _QCRON_HPP
#define _QCRON_HPP

#include <QObject>

class QCron : public QObject
{
    Q_OBJECT

public:
    QCron();
    QCron(QString & pattern);
    ~QCron();

    // Features.
    bool isValid() const
        { return _is_valid; }

signals:
    void activated();
    void deactivated();

private:
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

    struct QCronField
    {
        EField field;
        bool is_valid;
        Node * last_node;
        Node * root;

        QCronField()
            : is_valid(false)
            {}

        IntNode * parseInt(QString & str);
        RangeNode* parseRange(QString & str);
        EveryNode* parseEvery(QString & str);
        ListNode * parseList(QString & str);
        Node * parseNode(QString & str);
        void parse(QString&);

    };

    bool _is_valid;
    QCronField _fields[6];

    void _init();
    void _parsePattern(QString & pattern);
    void _parseField(QString & field_str,
                     EField field);
    QString _validCharacters(EField field);

};

#endif
