#ifndef CUTEFAMILY_FAMILYSEARCHREPLY_H
#define CUTEFAMILY_FAMILYSEARCHREPLY_H

#include "cutefamily.h"
#include <QObject>
#include <QByteArray>
#include <QVariant>

class QNetworkReply;

namespace CuteFamily
{
    class FamilySearch;

    class CUTEFAMILY_API FamilySearchReply : public QObject
    {
        Q_OBJECT
    public:
        enum Error
        {
            NoError,
            UnknownError
        };

    public:
        ~FamilySearchReply();

        Error error() const { return _error; }
        QByteArray data() const { return _data; }
        QVariantMap variantMap() const;

    signals:
        void finished();

    private slots:
        void networkReplyFinished();

    private:
        friend FamilySearch;
        FamilySearchReply(QNetworkReply *pReply);

    private:
        QNetworkReply *_pReply;
        QByteArray _data;
        Error _error;
    };
}

#endif // CUTEFAMILY_FAMILYSEARCHREPLY_H
