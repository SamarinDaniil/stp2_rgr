#ifndef TST_TFRAC_H
#define TST_TFRAC_H

#include <QObject>
#include <QtTest>
#include <QString>

#include "../FractionCalculator/tfrac.h"

class TestTFrac : public QObject
{
    Q_OBJECT

public:
    TestTFrac();
    ~TestTFrac();

private slots:
    void testInitFromString();
    void testInitFromString_data();

    void testInitFromNumber();
    void testInitFromNumber_data();

    void testAddition();
    void testAddition_data();

    void testSubtraction();
    void testSubtraction_data();

    void testMultiplication();
    void testMultiplication_data();

    void testDivision();
    void testDivision_data();

    void testSquare();
    void testSquare_data();

    void testReverse();
    void testReverse_data();

    void testEquality();
    void testEquality_data();

    void testGreaterThan();
    void testGreaterThan_data();

    void testDivisionByZero();
    void testReverseZero();

};

#endif // TST_TFRAC_H
