#ifndef _QCRON_HPP
#define _QCRON_HPP

#include <QObject>
#include <QDateTime>
#include "qcronfield.hpp"

class QCron : public QObject
{
    Q_OBJECT

public:
    QCron();
    QCron(QString & pattern);
    ~QCron();

    // Accessors.
    void setBeginning(const QDateTime & date_time)
        { _beginning = date_time; }

    bool isValid() const
        { return _is_valid; }

    // Features.

    QDateTime next(int n = 1) const;

signals:
    void activated();
    void deactivated();

private:
    bool _is_valid;
    QCronField _fields[6];
    QDateTime _beginning;

    void _init();
    void _parsePattern(QString & pattern);
    void _parseField(QString & field_str,
                     EField field);
    QString _validCharacters(EField field);

};

#endif
