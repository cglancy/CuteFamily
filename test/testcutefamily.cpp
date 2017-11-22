#include "testcutefamily.h"
#include "familysearch.h"
#include "familysearchreply.h"

// used to define your own FAMILYSEARCH_APP_KEY, FAMILYSEARCH_USERNAME, FAMILYSEARCH_PASSWORD
#include "familysearchsecret.h"

#include <QTest>
#include <QSharedPointer>
#include <QSignalSpy>
#include <QDebug>
#include <QScopedPointer>

using namespace CuteFamily;

QTEST_MAIN(TestCuteFamily)

void TestCuteFamily::initTestCase()
{
}

void TestCuteFamily::cleanupTestCase()
{
}

void TestCuteFamily::testOAuthUnauthenticated()
{
    QScopedPointer<FamilySearch> pFamilySearch(new FamilySearch());

    pFamilySearch->oauthUnauthenticated(FAMILYSEARCH_APP_KEY);
    QSignalSpy authSpy(pFamilySearch.data(), &FamilySearch::oauthFinished);
    QVERIFY(authSpy.wait(20000));

    QString token = pFamilySearch->accessToken();
    QVERIFY(!token.isEmpty());
}

void TestCuteFamily::testOAuthPassword()
{
    QScopedPointer<FamilySearch> pFamilySearch(new FamilySearch());

    pFamilySearch->oauthPassword(FAMILYSEARCH_USERNAME, FAMILYSEARCH_PASSWORD, FAMILYSEARCH_APP_KEY);
    QSignalSpy authSpy(pFamilySearch.data(), &FamilySearch::oauthFinished);
    QVERIFY(authSpy.wait(20000));

    QString token = pFamilySearch->accessToken();
    QVERIFY(!token.isEmpty());
}

void TestCuteFamily::testCurrentUser()
{
    QScopedPointer<FamilySearch> pFamilySearch(new FamilySearch());

    pFamilySearch->oauthPassword(FAMILYSEARCH_USERNAME, FAMILYSEARCH_PASSWORD, FAMILYSEARCH_APP_KEY);
    QSignalSpy authSpy(pFamilySearch.data(), &FamilySearch::oauthFinished);
    QVERIFY(authSpy.wait(20000));

    QScopedPointer<FamilySearchReply> pReply(pFamilySearch->get("/platform/users/current", FamilySearch::FamilySearchJsonMediaType));
    QSignalSpy replySpy(pReply.data(), &FamilySearchReply::finished);
    QVERIFY(replySpy.wait(5000));

    QVariantMap map = pReply->variantMap();
    QVariantList users = map["users"].toList();
    QVERIFY(users.size() == 1);
}

void TestCuteFamily::testPlaces()
{
    QScopedPointer<FamilySearch> pFamilySearch(new FamilySearch());

    pFamilySearch->oauthUnauthenticated(FAMILYSEARCH_APP_KEY);
    QSignalSpy authSpy(pFamilySearch.data(), &FamilySearch::oauthFinished);
    QVERIFY(authSpy.wait(20000));

    QScopedPointer<FamilySearchReply> pReply(pFamilySearch->get("/platform/places/search?q=name:Paris", FamilySearch::GedcomAtomJsonMediaType));
    QSignalSpy replySpy(pReply.data(), &FamilySearchReply::finished);
    QVERIFY(replySpy.wait(5000));

    QVariantMap map = pReply->variantMap();
    QVERIFY(map["title"] == "Place Search Results");
}

void TestCuteFamily::testPersonAncestry()
{
    QScopedPointer<FamilySearch> pFamilySearch(new FamilySearch());

    pFamilySearch->oauthPassword(FAMILYSEARCH_USERNAME, FAMILYSEARCH_PASSWORD, FAMILYSEARCH_APP_KEY);
    QSignalSpy authSpy(pFamilySearch.data(), &FamilySearch::oauthFinished);
    QVERIFY(authSpy.wait(20000));

    QScopedPointer<FamilySearchReply> pReply(pFamilySearch->get("/platform/tree/ancestry?person=KW43-NYT", FamilySearch::FamilySearchJsonMediaType));
    QSignalSpy replySpy(pReply.data(), &FamilySearchReply::finished);
    QVERIFY(replySpy.wait(5000));

    QVariantMap map = pReply->variantMap();
    QVariantList persons = map["persons"].toList();
    QVERIFY(persons.size() > 0);

    QVariantMap person0 = persons[0].toMap();
    QVERIFY(person0["id"] == "KW43-NYT");

    QVariantMap person0display = person0["display"].toMap();
    QVERIFY(person0display["ascendancyNumber"] == 1);
}