#ifndef COMMAND_TEST_HEADER
#define COMMAND_TEST_HEADER

#include <QtTest>
//initTestCase()        - called before the first test function is executed.
//initTestCase_data()   - called to create a global test data table.
//cleanupTestCase()     - called after the last test function was executed.
//init()                - called before each test function is executed.
//cleanup()             - called after every test function.

class CommandTest : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testEnterNumber();
    void testEnterNumberClone();

};
#endif
