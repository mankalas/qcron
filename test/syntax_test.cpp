#include "syntax_test.hpp"

#include <qcron.hpp>

#include <QDebug>
#include <QTest>

/******************************************************************************/

QString
SyntaxTest::
buildPattern(int pattern_idx, QString val)
{
    QString res;
    for (int i = 0; i < 6; ++i)
    {
        if (i == pattern_idx)
        {
            res.append(val).append(" ");
        }
        else
        {
            res.append("* ");
        }
    }
    return res;
}

/******************************************************************************/

void
SyntaxTest::
buildTests(QStringList & good,
           QStringList & bad,
           int pattern_idx,
           int min,
           int max)
{
    bad << buildPattern(pattern_idx, QString::number(min - 1)) <<
        buildPattern(pattern_idx, QString::number(max + 1)) <<
        buildPattern(pattern_idx, "a") <<
        buildPattern(pattern_idx, "1a") <<
        buildPattern(pattern_idx, QString("%2-%1").arg(min).arg(max)) <<
        buildPattern(pattern_idx, "2-a") <<
        buildPattern(pattern_idx, QString("%1-%2").arg(min).arg(max + 1)) <<
        buildPattern(pattern_idx, QString("%1-%2").arg(min - 1).arg(max)) <<
        buildPattern(pattern_idx, QString("%1,%2,").arg(min).arg(min + 1)) <<
        buildPattern(pattern_idx, "1,a") <<
        buildPattern(pattern_idx, QString("%1,%2").arg(min).arg(max + 1)) <<
        buildPattern(pattern_idx, QString("%1,%2").arg(min).arg(min - 1)) <<
        buildPattern(pattern_idx, "1/2/3 * * * * *") <<
        buildPattern(pattern_idx, "1-2-3 * * * * *") << // Bad range
        buildPattern(pattern_idx, "** * * * * *") << // Bad star
        buildPattern(pattern_idx, "1,,2 * * * * *") // Bad list
        ;
    good << buildPattern(pattern_idx, QString::number(min)) <<
         buildPattern(pattern_idx, QString("000000000%1").arg(min)) <<
         buildPattern(pattern_idx, QString::number(max)) <<
         buildPattern(pattern_idx, QString("%1-%2").arg(min).arg(max)) <<
         buildPattern(pattern_idx, QString("%1-%1").arg(min)) <<
         buildPattern(pattern_idx, QString("%1,%2,%3").arg(min).arg(min + 1).arg(max - 1)) <<
         buildPattern(pattern_idx, QString("%1,%1").arg(min));
}

/******************************************************************************/

void
SyntaxTest::
global()
{
    QStringList good = QStringList() <<
                       "* * * * * *" <<
                       "*         * * * * *" <<
                       "*\t* *\t\t * * *";
    QStringList bad = QStringList() <<
                      "* * * * *" << // Not enough fields
                      "* * * * * * *" << // To many fields
                      "* * *\n* * *"; // Bad separator
    doTest(good, bad);
}

/******************************************************************************/

void
SyntaxTest::
minutes()
{
    QStringList good;
    QStringList bad;
    buildTests(good, bad, 0, 0, 59);
    doTest(good, bad);
}

/******************************************************************************/

void
SyntaxTest::
hours()
{
    QStringList good;
    QStringList bad;
    buildTests(good, bad, 1, 0, 23);
    doTest(good, bad);
}

/******************************************************************************/

void
SyntaxTest::
daysOfMonth()
{
    QStringList good;
    QStringList bad;
    buildTests(good, bad, 2, 1, 31);
    doTest(good, bad);
}

/******************************************************************************/

void
SyntaxTest::
months()
{
    QStringList good;
    QStringList bad;
    buildTests(good, bad, 3, 1, 12);
    doTest(good, bad);
}

/******************************************************************************/

void
SyntaxTest::
daysOfWeek()
{
    QStringList good;
    QStringList bad;
    buildTests(good, bad, 4, 1, 7);
    doTest(good, bad);
}

/******************************************************************************/

void
SyntaxTest::
years()
{
    QStringList good;
    QStringList bad;
    buildTests(good, bad, 5, 1, 2099);
    doTest(good, bad);
}

/******************************************************************************/

void
SyntaxTest::
doTest(QStringList good, QStringList bad)
{
    foreach (QString s, good)
    {
        //        qDebug() << s;
        QCron cron(s);
        QVERIFY(cron.isValid());
    }
    foreach (QString s, bad)
    {
                qDebug() << s;
        QCron cron(s);
        QVERIFY(!cron.isValid());
    }
}

/******************************************************************************/

QTEST_MAIN(SyntaxTest)
