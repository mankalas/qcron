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
    QCron(const QString & pattern, bool isauto=true);
    ~QCron();

    // Accessors.
    bool isActive() const
        { return _is_active; }

    bool isValid() const
        { return _is_valid; }

    const QString & error() const
        { return _error; }

    const QString &command() const
        { return _command; }

    // Features.

    QDateTime next();
    QDateTime next(QDateTime dt);
    void catchUp(QDateTime & dt, EField field, int value);
    bool match(const QDateTime & dt) const;
    void add(QDateTime & dt, EField field, int value);

signals:
    void activated();
    void deactivated();

private:
    bool _is_auto;
    bool _is_valid;
    bool _is_active;
    QString _error;
    QCronField _fields[6];
    QString _command;

    void _init(bool isauto);
    void _setError(const QString & error);
    void _parsePattern(const QString & pattern);
    void _parseField(QString & field_str,
                     EField field);
    QString _validCharacters(EField field);
    void _process(QDateTime & dt, EField field);

public slots:
    void checkState();
};

#endif
