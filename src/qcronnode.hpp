#ifndef _QCRONNODE_HPP
#define _QCRONNODE_HPP

#include <QList>
#include <QtGlobal>

#include "qcronfield.hpp"

class QCron;
class QCronField;

class QCronNode
{
public:
    virtual ~QCronNode();

    void setField(QCronField * field);

    virtual int next(int t) const = 0;

    virtual bool match(const QDateTime & dt) const = 0;
    virtual void process(QCron * cron,
                         QDateTime & dt,
                         EField field) = 0;

protected:
    QCronField * _field;
};

/******************************************************************************/

class QCronValueNode : public QCronNode
{
};

/******************************************************************************/

class QCronIntNode : public QCronValueNode
{
public:
    QCronIntNode(int v);

    int value() const;

    virtual int next(int t) const Q_DECL_OVERRIDE;
    virtual bool match(const QDateTime & dt) const Q_DECL_OVERRIDE;

    virtual void process(QCron * cron,
                         QDateTime & dt,
                         EField field) Q_DECL_OVERRIDE;

private:
    int _value;
};

/******************************************************************************/

class QCronStrNode : public QCronValueNode
{
public:
    virtual int next(int t) const Q_DECL_OVERRIDE;
    virtual bool match(const QDateTime & dt) const Q_DECL_OVERRIDE;
    virtual void process(QCron * cron,
                         QDateTime & dt,
                         EField field) Q_DECL_OVERRIDE;
};

/******************************************************************************/

class QCronAllNode : public QCronValueNode
{
public:
    virtual int next(int t) const Q_DECL_OVERRIDE;
    virtual bool match(const QDateTime & dt) const Q_DECL_OVERRIDE;
    virtual void process(QCron * cron,
                         QDateTime & dt,
                         EField field) Q_DECL_OVERRIDE;
};

/******************************************************************************/

class QCronRangeNode : public QCronNode
{
public:
    QCronRangeNode(const QCronIntNode * begin,
                   const QCronIntNode * end);

    int beginValue() const;
    int endValue() const;

    virtual int next(int t) const Q_DECL_OVERRIDE;
    virtual bool match(const QDateTime & dt) const Q_DECL_OVERRIDE;

    virtual void process(QCron * cron,
                         QDateTime & dt,
                         EField field) Q_DECL_OVERRIDE;

private:
    const QCronIntNode * _begin;
    const QCronIntNode * _end;
};

/******************************************************************************/

class QCronEveryNode : public QCronNode
{
public:
    QCronEveryNode(QCronNode *, QCronIntNode *);

    virtual int next(int t) const Q_DECL_OVERRIDE;
    virtual bool match(const QDateTime & dt) const Q_DECL_OVERRIDE;

    virtual void process(QCron * cron,
                         QDateTime & dt,
                         EField field) Q_DECL_OVERRIDE;

private:
    QCronNode * _what;
    QCronIntNode * _freq;
};

/******************************************************************************/

class QCronListNode : public QCronNode
{
public:
    QList<QCronNode *> & nodes();

    virtual int next(int t) const Q_DECL_OVERRIDE;
    virtual bool match(const QDateTime & dt) const Q_DECL_OVERRIDE;
    virtual void process(QCron * cron,
                         QDateTime & dt,
                         EField field) Q_DECL_OVERRIDE;

private:
    QList<QCronNode*> _nodes;
};

/******************************************************************************/

class QCronHolidayNode : public QCronNode
{
public:
    virtual int next(int t) const;

    virtual bool match(const QDateTime & dt) const;
    virtual void process(QCron * cron,
                         QDateTime & dt,
                         EField field);
};

/******************************************************************************/

class QCronNotHolidayNode : public QCronNode
{
public:
    virtual int next(int t) const;

    virtual bool match(const QDateTime & dt) const;
    virtual void process(QCron * cron,
                         QDateTime & dt,
                         EField field);
};

/******************************************************************************/


#endif
