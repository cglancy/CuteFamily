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
#ifndef CGFAMILY_FAMILYSEARCH_P_H
#define CGFAMILY_FAMILYSEARCH_P_H
#pragma once

#include "familysearch.h"
#include <QObject>
#include <QString>
#include <QByteArray>

class QNetworkAccessManager;

namespace cg
{
    class FamilySearchPrivate : public QObject
    {
        Q_OBJECT
        Q_DISABLE_COPY(FamilySearchPrivate)
        Q_DECLARE_PUBLIC(FamilySearch)
        FamilySearch * const q_ptr;

    public:
        FamilySearchPrivate(FamilySearch *pFS, const QString &applicationKey, FamilySearch::Environment environment);

        static QByteArray acceptValue(FamilySearch::MediaType mediaType);
        QNetworkRequest buildRequest(const QString &apiRoute, FamilySearch::MediaType mediaType = FamilySearch::NoMediaType) const;
        QString ipAddress() const;
        QString accessTokenUrl() const;

        QNetworkAccessManager *pNam;
        FamilySearch::Environment environment;
        QByteArray userAgent;
        QString applicationKey, accessToken, baseUrl;

    public slots:
        void loginFinished();
        void logoutFinished();
    };
}

#endif // CGFAMILY_FAMILYSEARCH_P_H
