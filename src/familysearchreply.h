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
#ifndef CGFAMILY_FAMILYSEARCHREPLY_H
#define CGFAMILY_FAMILYSEARCHREPLY_H
#pragma once

#include "cgfamily.h"
#include <QObject>
#include <QByteArray>
#include <QVariant>
#include <QDateTime>

class QNetworkReply;

namespace cg
{
    class FamilySearch;
    class FamilySearchReplyPrivate;

    class CGFAMILY_API FamilySearchReply : public QObject
    {
        Q_OBJECT
        Q_DECLARE_PRIVATE(FamilySearchReply)
        Q_DISABLE_COPY(FamilySearchReply)
        FamilySearchReplyPrivate * const d_ptr;

        friend FamilySearch;
        FamilySearchReply(QNetworkReply *pReply);

    public:
        enum Error
        {
            NoError,
            UnknownError
        };

        ~FamilySearchReply();

        int statusCode() const;
        Error error() const;
        QByteArray data() const;
        QVariantMap variantMap() const;

        /**
         * Gets the ETag value from the reply header.
         * Only available for Persons, Releationships or Change History resources.
         * @return ETag value
         */
        QByteArray eTag() const;

        /**
        * Gets the Last-Modified value from the reply header.
        * Only available for Persons, Releationships or Change History resources.
        * @return QDateTime value
        */
        QDateTime lastModified() const;

    signals:
        void finished();
    };
}

#endif // CGFAMILY_FAMILYSEARCHREPLY_H
