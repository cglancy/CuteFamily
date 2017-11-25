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
#include "familysearch.h"
#include "familysearchreply.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkInterface>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

namespace CuteFamily {

    FamilySearch::FamilySearch(Environment environment, QObject *parent)
        : QObject(parent),
        _environment(environment)
    {
        _pNam = new QNetworkAccessManager(this);
        _userAgent = "CuteFamily 0.1";

        switch (environment)
        {
        case ProductionEnvironment:
            _baseUrl = "https://api.familysearch.org/";
            break;
        case BetaEnvironment:
            _baseUrl = "https://beta.familysearch.org/";
            break;
        case IntegrationEnvironment:
            _baseUrl = "https://integration.familysearch.org/";
            break;
        }
    }

    FamilySearch::~FamilySearch()
    {
    }

    QString FamilySearch::accessTokenUrl() const
    {
        QString urlStr;

        switch (_environment)
        {
        case ProductionEnvironment:
            urlStr = "https://ident.familysearch.org/cis-web/oauth2/v3/token";
            break;
        case BetaEnvironment:
            urlStr = "https://identbeta.familysearch.org/cis-web/oauth2/v3/token";
            break;
        case IntegrationEnvironment:
            urlStr = "https://identint.familysearch.org/cis-web/oauth2/v3/token";
            break;
        }

        return urlStr;
    }

    void FamilySearch::oauthUnauthenticated(const QString &applicationKey)
    {
        QString urlStr = accessTokenUrl();

        QUrlQuery query;
        query.addQueryItem("grant_type", "unauthenticated_session");
        query.addQueryItem("client_id", applicationKey);
        query.addQueryItem("ip_address", ipAddress());

        QByteArray content = query.toString(QUrl::FullyEncoded).toUtf8();

        QUrl url(urlStr);
        QNetworkRequest req(url);
        req.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));
        req.setRawHeader("Accept", "application/json");
        QNetworkReply *pReply = _pNam->post(req, content);
        connect(pReply, &QNetworkReply::finished, this, &FamilySearch::oauthReplyFinished);
    }

    void FamilySearch::oauthPassword(const QString &username, const QString &password, const QString &applicationKey)
    {
        QString urlStr = accessTokenUrl();

        QUrlQuery query;
        query.addQueryItem("grant_type", "password");
        query.addQueryItem("username", username);
        query.addQueryItem("password", password);
        query.addQueryItem("client_id", applicationKey);

        QByteArray content = query.toString(QUrl::FullyEncoded).toUtf8();

        QUrl url(urlStr);
        QNetworkRequest req(url);
        req.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));
        req.setRawHeader("Accept", "application/json");
        QNetworkReply *pReply = _pNam->post(req, content);
        connect(pReply, &QNetworkReply::finished, this, &FamilySearch::oauthReplyFinished);
    }

    void FamilySearch::oauthReplyFinished()
    {
        QNetworkReply *pReply = qobject_cast<QNetworkReply*>(sender());
        if (!pReply)
            return;

        int status = pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

        if (pReply->error() == QNetworkReply::NoError)
        {
            QByteArray data = pReply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(data);
            if (doc.isObject())
            {
                QJsonObject obj = doc.object();
                QJsonValue value = obj.value("access_token");
                _accessToken = value.toString().toUtf8();
            }
        }
        else
        {
            QByteArray data = pReply->readAll();
            qDebug() << "Error: " << status << ", " << data;
        }

        emit oauthFinished();

        pReply->deleteLater();
    }

    QNetworkRequest FamilySearch::buildRequest(const QString &apiRoute, MediaType mediaType) const
    {
        QUrl url(_baseUrl + apiRoute);
        QNetworkRequest req(url);

        req.setRawHeader("User-Agent", _userAgent);

        if (mediaType != NoMediaType)
            req.setRawHeader("Accept", acceptValue(mediaType));

        if (!_accessToken.isEmpty())
            req.setRawHeader("Authorization", "Bearer " + _accessToken.toUtf8());

        return req;
    }

    FamilySearchReply* FamilySearch::head(const QString &apiRoute, MediaType mediaType)
    {
        QNetworkRequest req = buildRequest(apiRoute, mediaType);
        QNetworkReply *pReply = _pNam->head(req);
        FamilySearchReply *fsReply = new FamilySearchReply(pReply);
        return fsReply;
    }

    FamilySearchReply* FamilySearch::get(const QString &apiRoute, MediaType mediaType)
    {
        QNetworkRequest req = buildRequest(apiRoute, mediaType);
        QNetworkReply *pReply = _pNam->get(req);
        FamilySearchReply *fsReply = new FamilySearchReply(pReply);
        return fsReply;
    }

    FamilySearchReply* FamilySearch::put(const QString &apiRoute, const QByteArray &content, MediaType mediaType)
    {
        QNetworkRequest req = buildRequest(apiRoute, mediaType);
        QNetworkReply *pReply = _pNam->put(req, content);
        FamilySearchReply *fsReply = new FamilySearchReply(pReply);
        return fsReply;
    }

    FamilySearchReply* FamilySearch::post(const QString &apiRoute, const QByteArray &content, MediaType mediaType)
    {
        QNetworkRequest req = buildRequest(apiRoute, mediaType);
        QNetworkReply *pReply = _pNam->post(req, content);
        FamilySearchReply *fsReply = new FamilySearchReply(pReply);
        return fsReply;
    }

    FamilySearchReply* FamilySearch::del(const QString &apiRoute)
    {
        QNetworkRequest req = buildRequest(apiRoute, NoMediaType);
        QNetworkReply *pReply = _pNam->deleteResource(req);
        FamilySearchReply *fsReply = new FamilySearchReply(pReply);
        return fsReply;
    }

    QByteArray FamilySearch::acceptValue(MediaType mediaType)
    {
        QByteArray value;

        switch (mediaType)
        {
        case ApplicationJsonMediaType:
            value = "application/json";
            break;
        case GedcomJsonMediaType:
            value = "application/x-gedcomx-v1+json";
            break;
        case GedcomRecordsJsonMediaType:
            value = "application/x-gedcomx-records-v1+json";
            break;
        case GedcomAtomJsonMediaType:
            value = "application/x-gedcomx-atom+json";
            break;
        case FamilySearchJsonMediaType:
            value = "application/x-fs-v1+json";
            break;
        }

        return value;
    }

    QString FamilySearch::ipAddress() const
    {
        QList<QHostAddress> list = QNetworkInterface::allAddresses();
        for (auto & hostAddress : list)
        {
            if (!hostAddress.isLoopback() && hostAddress.protocol() == QAbstractSocket::IPv4Protocol)
                return hostAddress.toString();
        }

        return QString();
    }
}
