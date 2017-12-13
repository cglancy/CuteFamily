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
#include "familysearch_p.h"
#include "familysearchreply.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkInterface>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QCoreApplication>

namespace cg {

    //
    // FamilySearchPrivate
    // 

    FamilySearchPrivate::FamilySearchPrivate(FamilySearch *pFS, const QString &appKey, FamilySearch::Environment env)
        : q_ptr(pFS),
        applicationKey(appKey),
        environment(env)
    {
        pNam = new QNetworkAccessManager(this);
        userAgent = QString("%1 %2").arg(QCoreApplication::applicationName())
            .arg(QCoreApplication::applicationVersion()).toUtf8();

        switch (environment)
        {
        case FamilySearch::ProductionEnvironment:
            baseUrl = "https://api.familysearch.org";
            break;
        case FamilySearch::BetaEnvironment:
            baseUrl = "https://beta.familysearch.org";
            break;
        case FamilySearch::IntegrationEnvironment:
            baseUrl = "https://api-integ.familysearch.org";
            break;
        }
    }

    QString FamilySearchPrivate::accessTokenUrl() const
    {
        QString urlStr;

        switch (environment)
        {
        case FamilySearch::ProductionEnvironment:
            urlStr = "https://ident.familysearch.org/cis-web/oauth2/v3/token";
            break;
        case FamilySearch::BetaEnvironment:
            urlStr = "https://identbeta.familysearch.org/cis-web/oauth2/v3/token";
            break;
        case FamilySearch::IntegrationEnvironment:
            urlStr = "https://identint.familysearch.org/cis-web/oauth2/v3/token";
            break;
        }

        return urlStr;
    }

    void FamilySearchPrivate::loginFinished()
    {
        Q_Q(FamilySearch);

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
                accessToken = value.toString().toUtf8();
            }
        }
        else
        {
            QByteArray data = pReply->readAll();
            qDebug() << "Error: " << status << ", " << data;
        }

        emit q->loginFinished(status);

        pReply->deleteLater();
    }

    void FamilySearchPrivate::logoutFinished()
    {
        Q_Q(FamilySearch);

        QNetworkReply *pReply = qobject_cast<QNetworkReply*>(sender());
        if (!pReply)
            return;

        int status = pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

        if (pReply->error() == QNetworkReply::NoError && status == 204)
        {
            accessToken.clear();
        }
        else
        {
            QByteArray data = pReply->readAll();
            qDebug() << "Error: " << status << ", " << data;
        }

        emit q->logoutFinished(status);

        pReply->deleteLater();
    }

    QNetworkRequest FamilySearchPrivate::buildRequest(const QString &apiRoute, FamilySearch::MediaType mediaType) const
    {
        QString fullUrlStr = baseUrl;
        if (apiRoute.startsWith("/"))
            fullUrlStr += apiRoute;
        else
            fullUrlStr += "/" + apiRoute;

        QUrl url(fullUrlStr);
        QNetworkRequest req(url);

        req.setRawHeader("User-Agent", userAgent);

        if (mediaType != FamilySearch::NoMediaType)
            req.setRawHeader("Accept", acceptValue(mediaType));

        if (!accessToken.isEmpty())
            req.setRawHeader("Authorization", "Bearer " + accessToken.toUtf8());

        return req;
    }

    QByteArray FamilySearchPrivate::acceptValue(FamilySearch::MediaType mediaType)
    {
        QByteArray value;

        switch (mediaType)
        {
        case FamilySearch::ApplicationJsonMediaType:
            value = "application/json";
            break;
        case FamilySearch::GedcomJsonMediaType:
            value = "application/x-gedcomx-v1+json";
            break;
        case FamilySearch::GedcomRecordsJsonMediaType:
            value = "application/x-gedcomx-records-v1+json";
            break;
        case FamilySearch::GedcomAtomJsonMediaType:
            value = "application/x-gedcomx-atom+json";
            break;
        case FamilySearch::FamilySearchJsonMediaType:
            value = "application/x-fs-v1+json";
            break;
        }

        return value;
    }

    QString FamilySearchPrivate::ipAddress() const
    {
        QList<QHostAddress> list = QNetworkInterface::allAddresses();
        for (auto & hostAddress : list)
        {
            if (!hostAddress.isLoopback() && hostAddress.protocol() == QAbstractSocket::IPv4Protocol)
                return hostAddress.toString();
        }

        return QString();
    }

    //
    // FamilySearch
    //

    FamilySearch::FamilySearch(const QString &applicationKey, Environment environment, QObject *parent)
        : QObject(parent),
        d_ptr(new FamilySearchPrivate(this, applicationKey, environment))
    {
    }

    FamilySearch::~FamilySearch()
    {
    }

    QString FamilySearch::applicationKey() const
    {
        Q_D(const FamilySearch);
        return d->applicationKey;
    }

    FamilySearch::Environment FamilySearch::environment() const
    {
        Q_D(const FamilySearch);
        return d->environment;
    }

    QString FamilySearch::accessToken() const
    {
        Q_D(const FamilySearch);
        return d->accessToken;
    }

    void FamilySearch::setAccessToken(const QString &accessToken)
    {
        Q_D(FamilySearch);
        d->accessToken = accessToken;
    }

    bool FamilySearch::isLoggedIn() const
    {
        Q_D(const FamilySearch);
        return !d->accessToken.isEmpty();
    }

    void FamilySearch::loginUnauthenticated()
    {
        Q_D(FamilySearch);

        QString urlStr = d->accessTokenUrl();

        QUrlQuery query;
        query.addQueryItem("grant_type", "unauthenticated_session");
        query.addQueryItem("client_id", d->applicationKey);
        query.addQueryItem("ip_address", d->ipAddress());

        QByteArray content = query.toString(QUrl::FullyEncoded).toUtf8();

        QUrl url(urlStr);
        QNetworkRequest req(url);
        req.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));
        req.setRawHeader("Accept", "application/json");
        QNetworkReply *pReply = d->pNam->post(req, content);
        connect(pReply, &QNetworkReply::finished, d, &FamilySearchPrivate::loginFinished);
    }

    void FamilySearch::login(const QString &username, const QString &password)
    {
        Q_D(FamilySearch);

        QString urlStr = d->accessTokenUrl();

        QUrlQuery query;
        query.addQueryItem("grant_type", "password");
        query.addQueryItem("username", username);
        query.addQueryItem("password", password);
        query.addQueryItem("client_id", d->applicationKey);

        QByteArray content = query.toString(QUrl::FullyEncoded).toUtf8();

        QUrl url(urlStr);
        QNetworkRequest req(url);
        req.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));
        req.setRawHeader("Accept", "application/json");
        QNetworkReply *pReply = d->pNam->post(req, content);
        connect(pReply, &QNetworkReply::finished, d, &FamilySearchPrivate::loginFinished);
    }

    void FamilySearch::logout()
    {
        Q_D(FamilySearch);

        if (!isLoggedIn())
            return;
            
        QString urlStr = d->baseUrl + "/platform/logout";
        QUrl url(urlStr);
        QNetworkRequest req(url);
        req.setRawHeader("Authorization", "Bearer " + d->accessToken.toUtf8());
        QNetworkReply *pReply = d->pNam->post(req, QByteArray());
        connect(pReply, &QNetworkReply::finished, d, &FamilySearchPrivate::logoutFinished);
    }

    FamilySearchReply* FamilySearch::head(const QString &apiRoute, MediaType mediaType)
    {
        Q_D(FamilySearch);
        QNetworkRequest req = d->buildRequest(apiRoute, mediaType);
        QNetworkReply *pReply = d->pNam->head(req);
        return new FamilySearchReply(pReply);
    }

    FamilySearchReply* FamilySearch::get(const QString &apiRoute, MediaType mediaType)
    {
        Q_D(FamilySearch);
        QNetworkRequest req = d->buildRequest(apiRoute, mediaType);
        QNetworkReply *pReply = d->pNam->get(req);
        return new FamilySearchReply(pReply);
    }

    FamilySearchReply* FamilySearch::put(const QString &apiRoute, const QByteArray &content, MediaType mediaType)
    {
        Q_D(FamilySearch);
        QNetworkRequest req = d->buildRequest(apiRoute, mediaType);
        QNetworkReply *pReply = d->pNam->put(req, content);
        return new FamilySearchReply(pReply);
    }

    FamilySearchReply* FamilySearch::post(const QString &apiRoute, const QByteArray &content, MediaType mediaType)
    {
        Q_D(FamilySearch);
        QNetworkRequest req = d->buildRequest(apiRoute, mediaType);
        QNetworkReply *pReply = d->pNam->post(req, content);
        return new FamilySearchReply(pReply);
    }

    FamilySearchReply* FamilySearch::del(const QString &apiRoute)
    {
        Q_D(FamilySearch);
        QNetworkRequest req = d->buildRequest(apiRoute, NoMediaType);
        QNetworkReply *pReply = d->pNam->deleteResource(req);
        return new FamilySearchReply(pReply);
    }
}
