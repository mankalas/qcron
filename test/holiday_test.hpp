#ifndef _HOLIDAY_TEST_HPP
#define _HOLIDAY_TEST_HPP

#include <QObject>

class HolidayTest : public QObject
{
    Q_OBJECT

private slots:
    void easter();
    void holiday();
};

#endif
