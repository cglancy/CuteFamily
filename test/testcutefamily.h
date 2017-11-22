#ifndef TEST_CUTEFAMILY_H
#define TEST_CUTEFAMILY_H

#include <QObject>

class TestCuteFamily : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void cleanupTestCase();

    void testOAuthUnauthenticated();
    void testOAuthPassword();
    void testCurrentUser();
    void testPlaces();
    void testPersonAncestry();

private:
};

#endif // TEST_CUTEFAMILY_H
