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
#ifndef CUTEFAMILY_FAMILYSEARCHREPLY_H
#define CUTEFAMILY_FAMILYSEARCHREPLY_H
#pragma once

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
