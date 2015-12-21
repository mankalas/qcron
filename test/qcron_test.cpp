#include "qcron_test.hpp"

#include <QTest>

/******************************************************************************/

void
QCronTest::
basics()
{
    QDateTime now = QDateTime::currentDateTime();
    QCron c("* * * * * *");
    c.setBeginning(now);
    QDateTime next = c.next();
    qDebug() << next;
    QFAIL("Nope!");
}

/******************************************************************************/

QTEST_MAIN(QCronTest)
