#include "familysearchreply.h"

#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

namespace CuteFamily {

    FamilySearchReply::FamilySearchReply(QNetworkReply *pReply)
        : _pReply(pReply),
        _error(NoError)
    {
        if (_pReply)
            connect(_pReply, &QNetworkReply::finished, this, &FamilySearchReply::networkReplyFinished);
    }

    FamilySearchReply::~FamilySearchReply()
    {
        if (_pReply)
            delete _pReply;
    }

    void FamilySearchReply::networkReplyFinished()
    {
        if (_pReply)
        {
            if (_pReply->error() == QNetworkReply::NoError)
            {
                _data = _pReply->readAll();
                //qDebug() << "Response: " << _data;
            }
            else
            {
                //dump error
                qDebug() << "Network Error: " << _pReply->error();
                qDebug() << "Response error: " << _pReply->readAll();
            }
        }

        emit finished();
    }

    QVariantMap FamilySearchReply::variantMap() const
    {
        QVariantMap map;

        QJsonDocument doc = QJsonDocument::fromJson(data());
        if (doc.isObject())
        {
            QJsonObject obj = doc.object();
            map = obj.toVariantMap();
        }

        return map;
    }
}
