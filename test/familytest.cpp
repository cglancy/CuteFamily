/**
* Copyright 2017 Charles Glancy (charles@glancyfamily.net)
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without restriction, including  without limitation the rights to use, copy,
* modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software
* is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
* WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
* COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "familytest.h"
#include "familysearch.h"
#include "familysearchreply.h"

// used to define your own FAMILYSEARCH_APP_KEY, FAMILYSEARCH_USERNAME, FAMILYSEARCH_PASSWORD
#include "familysearchsecret.h"

#include <QTest>
#include <QSharedPointer>
#include <QSignalSpy>
#include <QDebug>
#include <QScopedPointer>

using namespace cg;

QTEST_MAIN(FamilyTest)

void FamilyTest::initTestCase()
{
    // set name & version for user-agent
    QCoreApplication::setApplicationName("FamilyTest");
    QCoreApplication::setApplicationVersion("0.1");
}

void FamilyTest::cleanupTestCase()
{
}

void FamilyTest::testLoginUnauthenticated()
{
    QScopedPointer<FamilySearch> pFamilySearch(new FamilySearch(FAMILYSEARCH_APP_KEY));

    pFamilySearch->loginUnauthenticated();
    QSignalSpy authSpy(pFamilySearch.data(), &FamilySearch::loginFinished);
    QVERIFY(authSpy.wait(20000));

    QString token = pFamilySearch->accessToken();
    QVERIFY(!token.isEmpty());
    QVERIFY(pFamilySearch->isLoggedIn());

    pFamilySearch->logout();
    QSignalSpy logoutSpy(pFamilySearch.data(), &FamilySearch::logoutFinished);
    QVERIFY(logoutSpy.wait(20000));

    QVERIFY(pFamilySearch->isLoggedIn() == false);
}

void FamilyTest::testLogin()
{
    QScopedPointer<FamilySearch> pFamilySearch(new FamilySearch(FAMILYSEARCH_APP_KEY));

    pFamilySearch->login(FAMILYSEARCH_USERNAME, FAMILYSEARCH_PASSWORD);
    QSignalSpy authSpy(pFamilySearch.data(), &FamilySearch::loginFinished);
    QVERIFY(authSpy.wait(20000));

    QString token = pFamilySearch->accessToken();
    QVERIFY(!token.isEmpty());
    QVERIFY(pFamilySearch->isLoggedIn());

    pFamilySearch->logout();
    QSignalSpy logoutSpy(pFamilySearch.data(), &FamilySearch::logoutFinished);
    QVERIFY(logoutSpy.wait(20000));

    QVERIFY(pFamilySearch->isLoggedIn() == false);
}

void FamilyTest::testCurrentUser()
{
    QScopedPointer<FamilySearch> pFamilySearch(new FamilySearch(FAMILYSEARCH_APP_KEY));

    pFamilySearch->login(FAMILYSEARCH_USERNAME, FAMILYSEARCH_PASSWORD);
    QSignalSpy authSpy(pFamilySearch.data(), &FamilySearch::loginFinished);
    QVERIFY(authSpy.wait(20000));

    QScopedPointer<FamilySearchReply> pReply(pFamilySearch->get("/platform/users/current", FamilySearch::FamilySearchJsonMediaType));
    QSignalSpy replySpy(pReply.data(), &FamilySearchReply::finished);
    QVERIFY(replySpy.wait(5000));

    QVariantMap map = pReply->variantMap();
    QVariantList users = map["users"].toList();
    QVERIFY(users.size() == 1);
}

void FamilyTest::testPlaces()
{
    QScopedPointer<FamilySearch> pFamilySearch(new FamilySearch(FAMILYSEARCH_APP_KEY));

    pFamilySearch->loginUnauthenticated();
    QSignalSpy authSpy(pFamilySearch.data(), &FamilySearch::loginFinished);
    QVERIFY(authSpy.wait(20000));

    QScopedPointer<FamilySearchReply> pReply(pFamilySearch->get("/platform/places/search?q=name:Paris", FamilySearch::GedcomAtomJsonMediaType));
    QSignalSpy replySpy(pReply.data(), &FamilySearchReply::finished);
    QVERIFY(replySpy.wait(5000));

    QVariantMap map = pReply->variantMap();
    QVERIFY(map["title"] == "Place Search Results");
}

void FamilyTest::testPersonAncestry()
{
    QScopedPointer<FamilySearch> pFamilySearch(new FamilySearch(FAMILYSEARCH_APP_KEY));

    pFamilySearch->login(FAMILYSEARCH_USERNAME, FAMILYSEARCH_PASSWORD);
    QSignalSpy authSpy(pFamilySearch.data(), &FamilySearch::loginFinished);
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

void FamilyTest::testEtag()
{
    QScopedPointer<FamilySearch> pFamilySearch(new FamilySearch(FAMILYSEARCH_APP_KEY));

    pFamilySearch->login(FAMILYSEARCH_USERNAME, FAMILYSEARCH_PASSWORD);
    QSignalSpy authSpy(pFamilySearch.data(), &FamilySearch::loginFinished);
    QVERIFY(authSpy.wait(20000));

    QScopedPointer<FamilySearchReply> pReply(pFamilySearch->head("/platform/tree/persons/KW43-NYT", FamilySearch::FamilySearchJsonMediaType));
    QSignalSpy replySpy(pReply.data(), &FamilySearchReply::finished);
    QVERIFY(replySpy.wait(5000));

    QByteArray etag = pReply->eTag();
    QVERIFY(!etag.isEmpty());

    QDateTime lastModified = pReply->lastModified();
    QVERIFY(!lastModified.isNull());
}