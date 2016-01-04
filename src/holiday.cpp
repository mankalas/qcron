#include "holiday.hpp"

#include <QList>
#include <QDebug>

/******************************************************************************/

QList<QDate>
Holiday::
yearsHolidays(int year)
{
    QDate easter = Holiday::easter(year);
    QList<QDate> holidays  = QList<QDate>()
                             << QDate(year, 1, 1)     // Jour de l'an
                             << easter.addDays(1)  // Lundi de Paques
                             << easter.addDays(39) // Jeaudi de l'Ascension
                             << easter.addDays(50) // Lundi de Pentecote
                             << QDate(year, 5, 1)     // Fete du travail
                             << QDate(year, 5, 8)     // Fete de la victoire
                             << QDate(year, 7, 14)    // Fete nationale
                             << QDate(year, 8, 15)    // Assomption
                             << QDate(year, 11, 1)    // Toussaint
                             << QDate(year, 11, 11)   // Armistice de 1918
                             << QDate(year, 12, 25)   // Noel
                             ;
    return holidays;
}

/******************************************************************************/

bool
Holiday::
isHoliday(const QDate & today)
{
    return yearsHolidays(today.year()).contains(today);
}

/******************************************************************************/

QDate
Holiday::
easter(int y)
{
    /* Many thanks to http://www.henk-reints.nl/easter/index.htm */
    int a = y % 19 + 1;
    int b = y / 100 + 1;
    int c = (3 * b) / 4 - 12;
    int d = (8 * b + 5) / 25 - 5;
    int e = (y * 5) / 4 - 10 - c;
    int f = ((11 * a + 20 + d - c) % 30 + 30) % 30;
    if (24 == f || (25 == f && a > 11))
    {
        ++f;
    }
    int g = 44 - f;
    if (g < 21)
    {
        g += 30;
    }
    return QDate(y, 3, 1).addDays(g + 7 - (e + g) % 7 - 1);
}

/******************************************************************************/

QDate
Holiday::
next(const QDate & date)
{
    int year = date.year();
    QList<QDate> holidays = yearsHolidays(year);
    int days_before_holiday = date.daysTo(holidays.last());

    if (days_before_holiday <= 0)
    {
        /* 'date' is after xmas: next holiday is New Year's Eve next
           year. */
        return QDate(year + 1, 1, 1);
    }
    foreach (QDate holiday, holidays)
    {
        days_before_holiday = date.daysTo(holiday);
        if (days_before_holiday > 0)
        {
            return holiday;
        }
    }
    qDebug() << "Can't find a valid next holiday" << date;
    qFatal("Should not be here");
    return QDate();
}

/******************************************************************************/
