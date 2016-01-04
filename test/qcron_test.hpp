#ifndef _QCRON_TEST_HPP
#define _QCRON_TEST_HPP

#include <QObject>
#include <QDateTime>

class QCronTest : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void years();
    void dow();
    void hours();
    void minutes();
    void realLife();
    void holidays();
    void excludeHolidays();

private:
    QDate _dnow;
    QTime _tnow;

    QDateTime actual(QString & pattern);
    QDateTime now();

};

#endif
