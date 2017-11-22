#ifndef CUTEFAMILY_FAMILYSEARCH_H
#define CUTEFAMILY_FAMILYSEARCH_H

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
