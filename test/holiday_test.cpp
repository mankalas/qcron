#include "holiday_test.hpp"

#include <holiday.hpp>

#include <QTest>

void
HolidayTest::
easter()
{
    QCOMPARE(Holiday::easter(2000), QDate(2000, 4, 23));
    QCOMPARE(Holiday::easter(2001), QDate(2001, 4, 15));
    QCOMPARE(Holiday::easter(2002), QDate(2002, 3, 31));
    QCOMPARE(Holiday::easter(2003), QDate(2003, 4, 20));
    QCOMPARE(Holiday::easter(2004), QDate(2004, 4, 11));
    QCOMPARE(Holiday::easter(2005), QDate(2005, 3, 27));
    QCOMPARE(Holiday::easter(2006), QDate(2006, 4, 16));
    QCOMPARE(Holiday::easter(2007), QDate(2007, 4, 8));
    QCOMPARE(Holiday::easter(2008), QDate(2008, 3, 23));
    QCOMPARE(Holiday::easter(2009), QDate(2009, 4, 12));
    QCOMPARE(Holiday::easter(2010), QDate(2010, 4, 4));
    QCOMPARE(Holiday::easter(2011), QDate(2011, 4, 24));
    QCOMPARE(Holiday::easter(2012), QDate(2012, 4, 8));
    QCOMPARE(Holiday::easter(2013), QDate(2013, 3, 31));
    QCOMPARE(Holiday::easter(2014), QDate(2014, 4, 20));
    QCOMPARE(Holiday::easter(2015), QDate(2015, 4, 5));
    QCOMPARE(Holiday::easter(2016), QDate(2016, 3, 27));
    QCOMPARE(Holiday::easter(2017), QDate(2017, 4, 16));
    QCOMPARE(Holiday::easter(2018), QDate(2018, 4, 1));
    QCOMPARE(Holiday::easter(2019), QDate(2019, 4, 21));
    QCOMPARE(Holiday::easter(2020), QDate(2020, 4, 12));
    QCOMPARE(Holiday::easter(2021), QDate(2021, 4, 4));
    QCOMPARE(Holiday::easter(2022), QDate(2022, 4, 17));
    QCOMPARE(Holiday::easter(2023), QDate(2023, 4, 9));
    QCOMPARE(Holiday::easter(2024), QDate(2024, 3, 31));
    QCOMPARE(Holiday::easter(2025), QDate(2025, 4, 20));
    QCOMPARE(Holiday::easter(2026), QDate(2026, 4, 5));
    QCOMPARE(Holiday::easter(2027), QDate(2027, 3, 28));
    QCOMPARE(Holiday::easter(2028), QDate(2028, 4, 16));
    QCOMPARE(Holiday::easter(2029), QDate(2029, 4, 1));
    QCOMPARE(Holiday::easter(2030), QDate(2030, 4, 21));

}

/******************************************************************************/

void
HolidayTest::
holiday()
{
    QVERIFY(Holiday::isHoliday( QDate(2016, 1, 1)));
    QVERIFY(Holiday::isHoliday( QDate(2016, 5, 1)));
    QVERIFY(Holiday::isHoliday( QDate(2016, 5, 8)));
    QVERIFY(Holiday::isHoliday( QDate(2016, 7, 14)));
    QVERIFY(Holiday::isHoliday( QDate(2016, 8, 15)));
    QVERIFY(Holiday::isHoliday( QDate(2016, 11, 1)));
    QVERIFY(Holiday::isHoliday( QDate(2016, 11, 11)));
    QVERIFY(Holiday::isHoliday( QDate(2016, 12, 25)));
    QVERIFY(Holiday::isHoliday( QDate(2016, 3, 28))); // Lundi de Paques
    QVERIFY(Holiday::isHoliday( QDate(2016, 5, 5))); // Jeudi de l'Ascension
    QVERIFY(Holiday::isHoliday( QDate(2016, 5, 16))); // Lundi de Pentecote

    // Chuck Norris' birthday (should be a holiday though).
    QVERIFY(!Holiday::isHoliday(QDate(2016, 3, 10)));
}

/******************************************************************************/

void
HolidayTest::
nextHoliday()
{
    QDate today = QDate(2016, 1, 1);
    QCOMPARE(Holiday::next(today), QDate(2016, 3, 28));
    today = QDate(2016, 2, 1);
    QCOMPARE(Holiday::next(today), QDate(2016, 3, 28));
}

/******************************************************************************/

QTEST_MAIN(HolidayTest)
