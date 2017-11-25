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
#ifndef CUTEFAMILY_FAMILYSEARCH_H
#define CUTEFAMILY_FAMILYSEARCH_H
#pragma once

#include "cutefamily.h"
#include <QObject>
#include <QString>
#include <QByteArray>
#include <QNetworkRequest>

class QNetworkReply;
class QNetworkAccessManager;

namespace CuteFamily
{
    class FamilySearchReply;

    class CUTEFAMILY_API FamilySearch : public QObject
    {
        Q_OBJECT
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

    public:
        FamilySearch(Environment environment = IntegrationEnvironment, QObject *parent = nullptr);
        ~FamilySearch();

        Environment environment() const { return _environment; }

        QString accessToken() const { return _accessToken; }
        void setAccessToken(const QString &accessToken) { _accessToken = accessToken; }

        void oauthUnauthenticated(const QString &applicationKey);
        void oauthPassword(const QString &username, const QString &password, const QString &applicationKey);

        FamilySearchReply* head(const QString &apiRoute, MediaType mediaType = ApplicationJsonMediaType);
        FamilySearchReply* get(const QString &apiRoute, MediaType mediaType = ApplicationJsonMediaType);
        FamilySearchReply* put(const QString &apiRoute, const QByteArray &content, MediaType mediaType = ApplicationJsonMediaType);
        FamilySearchReply* post(const QString &apiRoute, const QByteArray &content, MediaType mediaType = ApplicationJsonMediaType);
        FamilySearchReply* del(const QString &apiRoute);

    signals:
        void oauthFinished();

    private slots:
        void oauthReplyFinished();

    private:
        static QByteArray acceptValue(MediaType mediaType);
        QNetworkRequest buildRequest(const QString &apiRoute, MediaType mediaType = NoMediaType) const;
        QString ipAddress() const;
        QString accessTokenUrl() const;

    private:
        Q_DISABLE_COPY(FamilySearch)

        QNetworkAccessManager *_pNam;
        Environment _environment;
        QByteArray _userAgent;
        QString _accessToken, _baseUrl;
    };
}

#endif // CUTEFAMILY_FAMILYSEARCH_H
