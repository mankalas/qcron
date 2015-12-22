#ifndef _QCRONFIELD_HPP
#define _QCRONFIELD_HPP

#include <QDateTime>
#include <QList>
#include <QString>
#include "qcronnode.hpp"

enum EField
{
    MINUTE,
    HOUR,
    DOM,
    MONTH,
    DOW,
    YEAR
};

class QCronFieldException
{
public:
    QCronFieldException(const QString & msg)
        : _msg(msg)
        {
        }

    const QString & msg() const
        { return _msg; }

private:
    QString _msg;
};

class QCronField
{
public:
    QCronField();

    // Accessors.
    void setField(EField field)
        {
            _field = field;
            switch (_field)
            {
            case MINUTE: _min = 0; _max = 59;      break;
            case HOUR:   _min = 0; _max = 23;      break;
            case DOM:    _min = 1; _max = 31;      break;
            case MONTH:  _min = 1; _max = 12;      break;
            case DOW:    _min = 1; _max = 7 ;      break;
            case YEAR:   _min = 2016; _max = 2099; break;
            default:     throw 42;
            }
        }

    // Features.
    void parse(QString & str);

    bool isValid() const
        { return _is_valid; }

    void next(QDateTime & dt);

    int getDateTimeSection(QDateTime & dt) const;
    void applyOffset(QDateTime & dt, int offset) const;

private:
    int _min;
    int _max;
    EField _field;
    bool _is_valid;
    QCronNode * _last_node;
    QCronNode * _root;

    QCronIntNode * _parseInt(QString & str);
    QCronRangeNode* _parseRange(QString & str);
    QCronEveryNode* _parseEvery(QString & str);
    QCronListNode * _parseList(QString & str);
    QCronNode * _parseNode(QString & str);
    int getTimeSection(QDateTime & dt) const;
};

#endif
