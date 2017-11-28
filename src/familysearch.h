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
#ifndef CGFAMILY_FAMILYSEARCH_H
#define CGFAMILY_FAMILYSEARCH_H
#pragma once

#include "cgfamily.h"
#include <QObject>
#include <QString>
#include <QByteArray>
#include <QNetworkRequest>

class QNetworkReply;
class QNetworkAccessManager;

namespace cg
{
    class FamilySearchPrivate;
    class FamilySearchReply;

    class CGFAMILY_API FamilySearch : public QObject
    {
        Q_OBJECT
        Q_DECLARE_PRIVATE(FamilySearch)
        Q_DISABLE_COPY(FamilySearch)
        FamilySearchPrivate * const d_ptr;

    public:
        enum Environment
        {
            IntegrationEnvironment,
            BetaEnvironment,
            ProductionEnvironment
        };

        enum MediaType
        {
            NoMediaType,
            ApplicationJsonMediaType,
            GedcomJsonMediaType,
            GedcomRecordsJsonMediaType,
            GedcomAtomJsonMediaType,
            FamilySearchJsonMediaType
        };

        FamilySearch(const QString &applicationKey, Environment environment = IntegrationEnvironment, QObject *parent = nullptr);
        ~FamilySearch();

        QString applicationKey() const;

        Environment environment() const;

        QString accessToken() const;
        void setAccessToken(const QString &accessToken);

        bool isLoggedIn() const;
        void login(const QString &username, const QString &password);
        void loginUnauthenticated();
        void logout();

        FamilySearchReply * head(const QString &apiRoute, MediaType mediaType = ApplicationJsonMediaType);
        FamilySearchReply * get(const QString &apiRoute, MediaType mediaType = ApplicationJsonMediaType);
        FamilySearchReply * put(const QString &apiRoute, const QByteArray &content, MediaType mediaType = ApplicationJsonMediaType);
        FamilySearchReply * post(const QString &apiRoute, const QByteArray &content, MediaType mediaType = ApplicationJsonMediaType);
        FamilySearchReply * del(const QString &apiRoute);

    signals:
        void loginFinished(int status);
        void logoutFinished(int status);
    };
}

#endif // CGFAMILY_FAMILYSEARCH_H
