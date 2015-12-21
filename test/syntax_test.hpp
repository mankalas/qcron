#ifndef _SYNTAX_TEST_HPP
#define _SYNTAX_TEST_HPP

#include <QObject>

class SyntaxTest : public QObject
{
    Q_OBJECT

private slots:
    void global();
    void minutes();
    void hours();
    void months();
    void years();
    void daysOfMonth();
    void daysOfWeek();

private:
    QString buildPattern(int pattern_idx,
                         QString val);
    void buildTests(QStringList & good,
                    QStringList & bad,
                    int pattern_idx,
                    int min,
                    int max);
    void doTest(QStringList good,
                QStringList bad);
};


#endif
