#include <QtTest>

#include "pointertemplates.h"

class PartialTemplateSpecialization : public QObject
{
    Q_OBJECT

public:
    PartialTemplateSpecialization();
    ~PartialTemplateSpecialization();

private slots:
    void testIsPointer();
    void testIsNotPointer();
    void testIsSecondOrderPointer();
    void testIsNotSecondOrderPointer();

private:
    struct Point
    {
        Point(double x, double y)
            : mX{x}
            , mY{y}
        {
        }

        double mX;
        double mY;
    };

    int mInt;
    int* mIntPtr;
    int** mSecondOrderIntPtr;
    Point mPoint;
    Point* mPointPtr;
    Point** mSecondOrderPointPtr;
};

PartialTemplateSpecialization::PartialTemplateSpecialization()
    : mInt{5}
    , mPoint{2.0, -9.0}
{
    mIntPtr = &mInt;
    mSecondOrderIntPtr = &mIntPtr;
    mPointPtr = &mPoint;
    mSecondOrderPointPtr = &mPointPtr;
}

PartialTemplateSpecialization::~PartialTemplateSpecialization()
{
}

void PartialTemplateSpecialization::testIsPointer()
{
    QVERIFY(isPointer(mIntPtr));
    QVERIFY(isPointer(mSecondOrderIntPtr));
    QVERIFY(isPointer(mPointPtr));
    QVERIFY(isPointer(mSecondOrderPointPtr));
}

void PartialTemplateSpecialization::testIsNotPointer()
{
    QVERIFY(!isPointer(mInt));
    QVERIFY(!isPointer(mPoint));
}

void PartialTemplateSpecialization::testIsSecondOrderPointer()
{
    QVERIFY(isSecondOrderPointer(mSecondOrderIntPtr));
    QVERIFY(isSecondOrderPointer(mSecondOrderPointPtr));
}

void PartialTemplateSpecialization::testIsNotSecondOrderPointer()
{
    QVERIFY(!isSecondOrderPointer(mInt));
    QVERIFY(!isSecondOrderPointer(mPoint));
    QVERIFY(!isSecondOrderPointer(mIntPtr));
    QVERIFY(!isSecondOrderPointer(mPointPtr));
}

QTEST_APPLESS_MAIN(PartialTemplateSpecialization)

#include "tst_partialtemplatespecialization.moc"
