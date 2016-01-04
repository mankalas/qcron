#ifndef _HOLIDAY_HPP
#define _HOLIDAY_HPP

#include <QDate>

class Holiday
{
public:
    static QList<QDate> yearsHolidays(int year);
    static bool isHoliday(const QDate & date);
    static QDate easter(int year);
    static QDate next(const QDate & date);
};

#endif
