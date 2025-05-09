/*
 * Copyright 2013 Canonical Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Loïc Molinari <loic.molinari@canonical.com>
 */

#ifndef UCURIHANDLER_P_H
#define UCURIHANDLER_P_H

#include <QtCore/QObject>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <LomiriToolkitQt6/lomiritoolkitglobal.h>
#else
#include <LomiriToolkit/lomiritoolkitglobal.h>
#endif

UT_NAMESPACE_BEGIN

class UCUriHandler;

class LOMIRITOOLKIT_EXPORT UriHandlerObject : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.freedesktop.Application")

 public:
    UriHandlerObject(UT_PREPEND_NAMESPACE(UCUriHandler)* uriHandler);

 public Q_SLOTS:
    void Open(const QStringList& uris, const QHash<QString, QVariant>& platformData);

 private:
    UCUriHandler* m_uriHandler;
};

class LOMIRITOOLKIT_EXPORT UCUriHandler : public QObject
{
    Q_OBJECT

 public:
    UCUriHandler();

 Q_SIGNALS:
    void opened(const QStringList& uris);

 private:
    UriHandlerObject m_uriHandlerObject;
};

UT_NAMESPACE_END

#endif // USURIHANDLER_P_H
