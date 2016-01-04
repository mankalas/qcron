#ifndef _HOLIDAY_HPP
#define _HOLIDAY_HPP

#include <QDate>

class Holiday
{
public:
    static bool isHoliday(const QDate & date);
    static QDate easter(int year);
};

#endif
