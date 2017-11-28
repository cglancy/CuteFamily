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
#include "familysearchreply.h"
#include "familysearchreply_p.h"

#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

namespace cg {

    //
    // FamilySearchReplyPrivate
    //

    FamilySearchReplyPrivate::FamilySearchReplyPrivate(FamilySearchReply *pFsReply, QNetworkReply *pNetworkReply)
        : q_ptr(pFsReply),
        pReply(pNetworkReply),
        error(FamilySearchReply::NoError)
    {
        if (pReply)
            connect(pReply, &QNetworkReply::finished, this, &FamilySearchReplyPrivate::replyFinished);
    }

    FamilySearchReplyPrivate::~FamilySearchReplyPrivate()
    {
        if (pReply)
            delete pReply;
    }

    void FamilySearchReplyPrivate::replyFinished()
    {
        Q_Q(FamilySearchReply);

        if (pReply)
        {
            if (pReply->error() == QNetworkReply::NoError)
            {
                data = pReply->readAll();
                //qDebug() << "Response: " << _data;
                //QByteArrayList list = pReply->rawHeaderList();
                //qDebug() << "Headers = " << list;
            }
            else
            {
                //dump error
                qDebug() << "Network Error: " << pReply->error();
                qDebug() << "Response error: " << pReply->readAll();
            }
        }

        emit q->finished();
    }

    //
    // FamilySearchReply
    //

    FamilySearchReply::FamilySearchReply(QNetworkReply *pReply)
        : d_ptr(new FamilySearchReplyPrivate(this, pReply))
    {
    }

    FamilySearchReply::~FamilySearchReply()
    {
    }

    int FamilySearchReply::statusCode() const
    {
        Q_D(const FamilySearchReply);
        return d->pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    }


    FamilySearchReply::Error FamilySearchReply::error() const
    {
        Q_D(const FamilySearchReply);
        return d->error;
    }

    QByteArray FamilySearchReply::data() const
    {
        Q_D(const FamilySearchReply);
        return d->data;
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

    QByteArray FamilySearchReply::eTag() const
    {
        Q_D(const FamilySearchReply);

        QByteArray etag;

        if (d->pReply && d->pReply->hasRawHeader("ETag"))
            etag = d->pReply->rawHeader("ETag");

        return etag;
    }

    QDateTime FamilySearchReply::lastModified() const
    {
        Q_D(const FamilySearchReply);

        QDateTime dateTime;

        if (d->pReply && d->pReply->hasRawHeader("Last-Modified"))
            dateTime = QDateTime::fromString(d->pReply->rawHeader("Last-Modified"), Qt::RFC2822Date);

        return dateTime;
    }
}
