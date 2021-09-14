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
    const int* mConstIntPtr;
    int* const mIntConstPtr;
    const int* const mConstIntConstPtr;
    int** mSecondOrderIntPtr;
    Point mPoint;
    Point* mPointPtr;
    const Point* mConstPointPtr;
    Point* const mPointConstPtr;
    const Point* const mConstPointConstPtr;
    Point** mSecondOrderPointPtr;
};

PartialTemplateSpecialization::PartialTemplateSpecialization()
    : mInt{5}
    , mIntPtr{new int{5}}
    , mConstIntPtr{new int{5}}
    , mIntConstPtr{new int{5}}
    , mConstIntConstPtr{new int{5}}
    , mSecondOrderIntPtr{new int*{new int{5}}}
    , mPoint{2.0, -9.0}
    , mPointPtr{new Point{2.0, -9.0}}
    , mConstPointPtr{new Point{2.0, -9.0}}
    , mPointConstPtr{new Point{2.0, -9.0}}
    , mConstPointConstPtr{new Point{2.0, -9.0}}
    , mSecondOrderPointPtr{new Point*{new Point{2.0, -9.0}}}
{
}

PartialTemplateSpecialization::~PartialTemplateSpecialization()
{
    delete mIntPtr;
    delete mConstIntPtr;
    delete mIntConstPtr;
    delete mConstIntConstPtr;
    delete *mSecondOrderIntPtr;
    delete mSecondOrderIntPtr;
    delete mPointPtr;
    delete mConstPointPtr;
    delete mPointConstPtr;
    delete mConstPointConstPtr;
    delete *mSecondOrderPointPtr;
    delete mSecondOrderPointPtr;
}

void PartialTemplateSpecialization::testIsPointer()
{
    QVERIFY(isPointer(mIntPtr));
    QVERIFY(isPointer(mConstIntPtr));
    QVERIFY(isPointer(mIntConstPtr));
    QVERIFY(isPointer(mConstIntConstPtr));
    QVERIFY(isPointer(mSecondOrderIntPtr));
    QVERIFY(isPointer(mPointPtr));
    QVERIFY(isPointer(mConstPointPtr));
    QVERIFY(isPointer(mPointConstPtr));
    QVERIFY(isPointer(mConstPointConstPtr));
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
