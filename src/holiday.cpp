#include "holiday.hpp"

#include <QList>

/******************************************************************************/

bool
Holiday::
isHoliday(const QDate & today)
{
    int y = today.year();
    QList<QDate> holidays  = QList<QDate>()
                             << QDate(y, 1, 1)   // Jour de l'an
                             << QDate(y, 5, 1)   // Fete du travail
                             << QDate(y, 5, 8)   // Fete de la victoire
                             << QDate(y, 7, 14)  // Fete nationale
                             << QDate(y, 8, 15)  // Assomption
                             << QDate(y, 11, 1)  // Toussaint
                             << QDate(y, 11, 11) // Armistice de 1918
                             << QDate(y, 12, 25) // Noel
                             ;
    QDate easter = Holiday::easter(y);

    return holidays.contains(today) ||
           today == easter.addDays(1) ||  // Lundi de Paques
           today == easter.addDays(39) || // Jeaudi de l'Ascension
           today == easter.addDays(50)    // Lundi de Pentecote
           ;
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
