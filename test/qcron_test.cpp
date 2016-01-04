#include "qcron_test.hpp"

#include <qcron.hpp>

#include <QDebug>
#include <QTest>

/******************************************************************************/

void
QCronTest::
init()
{
    _dnow = QDate(1, 1, 1);
    _tnow = QTime(0, 0, 0);
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
now()
{
    return QDateTime(_dnow, _tnow);
}

/******************************************************************************/

void
QCronTest::
minutes()
{
    // Star
    QString pattern = "* * * * * *";
    _tnow.setHMS(0, 0, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 1));

    // Int
    pattern = "30 * * * * *";
    QCOMPARE(actual(pattern), now().addSecs(60 * 30));
    _tnow.setHMS(0, 31, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 59));
    _tnow.setHMS(0, 30, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 60));

    pattern = "0 * * * * *";
    _tnow.setHMS(0, 0, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 60));
    _tnow.setHMS(0, 31, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 29));
    _tnow.setHMS(0, 59, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 1));

    pattern = "59 * * * * *";
    _tnow.setHMS(0, 0, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 59));
    _tnow.setHMS(0, 58, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 1));
    _tnow.setHMS(0, 59, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 60));

    // Range
    pattern = "10-20 * * * * *";
    _tnow.setHMS(0, 5, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 5));
    _tnow.setHMS(0, 9, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 1));
    _tnow.setHMS(0, 10, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 1));
    _tnow.setHMS(0, 15, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 1));
    _tnow.setHMS(0, 20, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 50));
    _tnow.setHMS(0, 30, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 40));

    // Every - easy
    pattern = "*/5 * * * * *";
    _tnow.setHMS(0, 0, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 5));
    _tnow.setHMS(0, 4, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 1));
    _tnow.setHMS(0, 5, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 5));
    _tnow.setHMS(0, 16, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 4));
    _tnow.setHMS(0, 54, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 1));
    _tnow.setHMS(0, 57, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 3));

    // Every - medium
    pattern = "14-28/2 * * * * *";
    _tnow.setHMS(0, 0, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 14));
    _tnow.setHMS(0, 13, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 1));
    _tnow.setHMS(0, 14, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 2));
    _tnow.setHMS(0, 15, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 1));
    _tnow.setHMS(0, 16, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 2));
    _tnow.setHMS(0, 28, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 46));

    // List - easy
    pattern = "10,15,16 * * * * *";
    _tnow.setHMS(0, 5, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 5));
    _tnow.setHMS(0, 9, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 1));
    _tnow.setHMS(0, 10, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 5));
    _tnow.setHMS(0, 14, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 1));
    _tnow.setHMS(0, 15, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 1));
    _tnow.setHMS(0, 16, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 54));
    _tnow.setHMS(0, 17, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 53));

    // List - medium
    pattern = "10,15-20,30 * * * * *";
    _tnow.setHMS(0, 5, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 5));
    _tnow.setHMS(0, 9, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 1));
    _tnow.setHMS(0, 10, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 5));
    _tnow.setHMS(0, 15, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 1));
    _tnow.setHMS(0, 19, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 1));
    _tnow.setHMS(0, 20, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 10));
    _tnow.setHMS(0, 30, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 40));
}

/******************************************************************************/


void
QCronTest::
years()
{
    // Int
    QString pattern = "* * * * * 30";
    _dnow.setDate(15, 1, 2);
    QCOMPARE(actual(pattern), now().addYears(15).addDays(-1));
    _dnow.setDate(31, 1, 1);
    QCOMPARE(actual(pattern), QDateTime());
    _dnow.setDate(30, 1, 1);
    QCOMPARE(actual(pattern), now().addSecs(60));
    _dnow.setDate(30, 12, 31);
    _tnow.setHMS(23, 59, 0);
    QCOMPARE(actual(pattern), QDateTime());

    pattern = "* * * 6 * 30";
    _dnow.setDate(15, 1, 1);
    _tnow.setHMS(0, 0, 0);
    QCOMPARE(actual(pattern), QDateTime(QDate(30, 6, 1), _tnow));
    _dnow.setDate(31, 1, 1);
    QCOMPARE(actual(pattern), QDateTime());
    _dnow.setDate(30, 6, 1);
    QCOMPARE(actual(pattern), now().addSecs(60));

    // Range
    pattern = "* * * * * 10-20";
    _dnow.setDate(5, 1, 1);
    QCOMPARE(actual(pattern), now().addYears(5));
    _dnow.setDate(9, 1, 1);
    QCOMPARE(actual(pattern), now().addYears(1));
    _dnow.setDate(10, 1, 1);
    QCOMPARE(actual(pattern), now().addSecs(60));
    _dnow.setDate(15, 1, 1);
    QCOMPARE(actual(pattern), now().addSecs(60));
    _dnow.setDate(20, 12, 31);
    _tnow.setHMS(23, 59, 0);
    QCOMPARE(actual(pattern), QDateTime());
    _dnow.setDate(20, 12, 31);
    QCOMPARE(actual(pattern), QDateTime());

    // Every - easy
    pattern = "* * * * * */5";
    _dnow.setDate(1, 1, 1);
    _tnow.setHMS(0, 0, 0);
    QCOMPARE(actual(pattern), now().addYears(4));
    _dnow.setDate(4, 1, 1);
    QCOMPARE(actual(pattern), now().addYears(1));
    _dnow.setDate(5, 1, 1);
    QCOMPARE(actual(pattern), now().addSecs(60));
    _dnow.setDate(16, 1, 1);
    QCOMPARE(actual(pattern), now().addYears(4));
    _dnow.setDate(54, 1, 1);
    QCOMPARE(actual(pattern), now().addYears(1));
    _dnow.setDate(57, 1, 1);
    QCOMPARE(actual(pattern), now().addYears(3));

    // Every - medium
    pattern = "* * * * * 14-28/2";
    _dnow.setDate(1, 1, 1);
    QCOMPARE(actual(pattern), now().addYears(13));
    _dnow.setDate(13, 1, 1);
    QCOMPARE(actual(pattern), now().addYears(1));
    _dnow.setDate(14, 1, 1);
    QCOMPARE(actual(pattern), now().addSecs(60));
    _dnow.setDate(15, 1, 1);
    QCOMPARE(actual(pattern), now().addYears(1));
    _dnow.setDate(16, 1, 1);
    QCOMPARE(actual(pattern), now().addSecs(60));
    _dnow.setDate(28, 12, 31);
    QCOMPARE(actual(pattern), now().addSecs(60));
    _tnow.setHMS(23, 59, 0);
    QCOMPARE(actual(pattern), QDateTime());

    // List - easy
    pattern = "* * * * * 10,15,16";
    _dnow.setDate(5, 1, 1);
    _tnow.setHMS(0, 0, 0);
    QCOMPARE(actual(pattern), now().addYears(5));
    _dnow.setDate(9, 1, 1);
    QCOMPARE(actual(pattern), now().addYears(1));
    _dnow.setDate(10, 1, 1);
    QCOMPARE(actual(pattern), now().addSecs(60));
    _dnow.setDate(14, 1, 1);
    QCOMPARE(actual(pattern), now().addYears(1));
    _dnow.setDate(15, 1, 1);
    QCOMPARE(actual(pattern), now().addSecs(60));
    _dnow.setDate(16, 1, 1);
    QCOMPARE(actual(pattern), now().addSecs(60));
    _dnow.setDate(16, 12, 31);
    _tnow.setHMS(23, 59, 00);
    QCOMPARE(actual(pattern), QDateTime());

    // List - medium
    pattern = "* * * * * 10,15-20,30";
    _dnow.setDate(5, 1, 1);
    _tnow.setHMS(0, 0, 0);
    QCOMPARE(actual(pattern), now().addYears(5));
    _dnow.setDate(9, 1, 1);
    QCOMPARE(actual(pattern), now().addYears(1));
    _dnow.setDate(10, 1, 1);
    QCOMPARE(actual(pattern), now().addSecs(60));
    _dnow.setDate(15, 1, 1);
    QCOMPARE(actual(pattern), now().addSecs(60));
    _dnow.setDate(20, 1, 1);
    QCOMPARE(actual(pattern), now().addSecs(60));
    _dnow.setDate(20, 12, 31);
    _tnow.setHMS(23, 59, 0);
    QCOMPARE(actual(pattern), now().addSecs(60).addYears(9));

    // Star
    pattern = "* * * * * *";
    _dnow.setDate(1, 1, 1);
    _tnow.setHMS(0, 0, 0);
    QCOMPARE(actual(pattern), now().addSecs(60));
}

/******************************************************************************/

void
QCronTest::
hours()
{
    // Int
    QString pattern = "* 1 * * * *";
    _dnow.setDate(2016, 1, 1);
    _tnow.setHMS(0, 0, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 60));
    _tnow.setHMS(0, 59, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 1));
    _tnow.setHMS(1, 0, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 1));
    _tnow.setHMS(1, 58, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 1));
    _tnow.setHMS(1, 59, 0);
    QCOMPARE(actual(pattern), now().addDays(1).addSecs(-60 * 59));

    pattern = "30 1,3 * * * *";
    _tnow.setHMS(0, 0, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 90));
    _tnow.setHMS(1, 30, 0);
    QCOMPARE(actual(pattern), now().addSecs(60 * 120));
    _tnow.setHMS(3, 30, 0);
    QCOMPARE(actual(pattern), now().addDays(1).addSecs(-120 * 60));

    pattern = "* * * * * 2017";
    _tnow.setHMS(0, 0, 0);
    QCOMPARE(actual(pattern), now().addYears(1));

    pattern = "* 2 * * * 2017";
    QCOMPARE(actual(pattern), now().addYears(1).addSecs(60 * 120));
}

/******************************************************************************/

void
QCronTest::
dow()
{
    // 1 = Mon; 7 = Sun
    QString pattern = "* * * * 1 *"; // Mon

    _dnow.setDate(2016, 1, 1); // Fri
    _tnow.setHMS(0, 0, 0);
    QCOMPARE(actual(pattern), now().addDays(3));

    pattern = "* * * * 5 *"; // Fri
    QCOMPARE(actual(pattern), now().addSecs(60));
    _tnow.setHMS(23, 59, 59);
    QCOMPARE(actual(pattern), now().addSecs(1).addDays(6));

    pattern = "* * * * 3 *"; // Wed
    _tnow.setHMS(0, 0, 0);
    QCOMPARE(actual(pattern), now().addDays(5));
}

/******************************************************************************/

void
QCronTest::
realLife()
{
    /* From monday to friday, from 8:00 to 11:59 and from 14:00 to 17:59. */
    QString working_hours = "* 8-11,14-17 * * 1-5 *";
    _dnow.setDate(2016, 1, 4); // Mon
    _tnow.setHMS(0, 0, 0);
    QCOMPARE(actual(working_hours), now().addSecs(8 * 3600));
    _tnow.setHMS(7, 59, 59);
    QCOMPARE(actual(working_hours), now().addSecs(60));
    _tnow.setHMS(8, 0, 0);
    QCOMPARE(actual(working_hours), now().addSecs(60));
    _tnow.setHMS(11, 58, 0);
    QCOMPARE(actual(working_hours), now().addSecs(60));
    _tnow.setHMS(11, 59, 0);
    QCOMPARE(actual(working_hours), now().addSecs(60 + 3600 * 2));
    _tnow.setHMS(13, 59, 0);
    QCOMPARE(actual(working_hours), now().addSecs(60));
    _tnow.setHMS(14, 0, 0);
    QCOMPARE(actual(working_hours), now().addSecs(60));
    _tnow.setHMS(17, 58, 0);
    QCOMPARE(actual(working_hours), now().addSecs(60));
    _tnow.setHMS(17, 59, 0);
    QCOMPARE(actual(working_hours), now().addSecs(60 + 14 * 3600));

    _dnow.setDate(2016, 1, 8); // Fri
    QCOMPARE(actual(working_hours), now().addSecs(60 + 14 * 3600).addDays(2));

}

/******************************************************************************/

QTEST_MAIN(QCronTest)
