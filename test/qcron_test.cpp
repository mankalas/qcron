#include "qcron_test.hpp"

#include <qcron.hpp>

#include <QDebug>
#include <QTest>

/******************************************************************************/

void
QCronTest::
init()
{
    _dnow = QDate::currentDate();
    _tnow = QTime(0,0,0);
}

/******************************************************************************/

QDateTime
QCronTest::
actual(QString & pattern)
{
    QCron c(pattern);
    c.setBeginning(QDateTime(_dnow, _tnow));
    return c.next();
}

/******************************************************************************/

QDateTime
QCronTest::
expected(int offset)
{
    QDateTime exp(_dnow, _tnow);
    return exp.addSecs(offset * 60);
}

/******************************************************************************/

void
QCronTest::
minutes()
{
    QString pattern = "* * * * * *";
    QCOMPARE(actual(pattern), expected(1));

    pattern = "30 * * * * *";
    QCOMPARE(actual(pattern), expected(30));
    _tnow.setHMS(0, 31, 0);
    QCOMPARE(actual(pattern), expected(59));
    _tnow.setHMS(0, 30, 0);
    QCOMPARE(actual(pattern), expected(60));
}

/******************************************************************************/

QTEST_MAIN(QCronTest)
