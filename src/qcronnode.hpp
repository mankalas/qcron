#ifndef _QCRONNODE_HPP
#define _QCRONNODE_HPP

#include <QList>
#include <QtGlobal>

class QCronNode
{
public:
    virtual ~QCronNode();

    virtual int next(int t) const = 0;
};

class QCronValueNode : public QCronNode
{
};

class QCronIntNode : public QCronValueNode
{
public:
    QCronIntNode(int v);

    int value() const;

    virtual int next(int t) const Q_DECL_OVERRIDE;

private:
    int _value;
};

class QCronStrNode : public QCronValueNode
{
public:
    virtual int next(int t) const Q_DECL_OVERRIDE;
};

class QCronAllNode : public QCronValueNode
{
public:
    virtual int next(int t) const Q_DECL_OVERRIDE;
};

class QCronRangeNode : public QCronNode
{
public:
    QCronRangeNode(const QCronIntNode * begin,
                   const QCronIntNode * end);

    virtual int next(int t) const Q_DECL_OVERRIDE;

private:
    const QCronIntNode * _begin;
    const QCronIntNode * _end;
};

class QCronEveryNode : public QCronNode
{
public:
    QCronEveryNode(QCronNode *, QCronIntNode *);

    virtual int next(int t) const Q_DECL_OVERRIDE;

private:
    QCronNode * _what;
    QCronIntNode * _freq;
};

class QCronListNode : public QCronNode
{
public:
    QList<QCronNode *> nodes() const;

    virtual int next(int t) const Q_DECL_OVERRIDE;

private:
    QList<QCronNode*> _nodes;
};

#endif
