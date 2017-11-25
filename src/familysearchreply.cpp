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
