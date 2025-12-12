/*
 * Copyright 2015 Canonical Ltd.
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
 * Author: Christian Dywan <christian.dywan@canonical.com>
 */

#include "plugin.h"

#include <QtQuick/private/qquickimagebase_p.h>
#include <QFile>
#include <QFileInfo>
#include <QUrl>

#include "europe.h"
#include "pangaea.h"
#include "rune.h"
#include "smilodon.h"
#include "tarpan.h"

static QObject *registerPangaea(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return new EAPangaea();
}

static QObject *registerPangaea4(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return new EAPangaea4();
}

void ExtinctAnimalsPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    Q_UNUSED(uri);
    Q_UNUSED(engine);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    Q_INIT_RESOURCE(ea_resources);
#endif
}

void ExtinctAnimalsPlugin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("Extinct.Animals"));

    QString base;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    // In Qt6, QQmlExtensionPlugin::baseUrl() is deprecated.
    // Finding the absolute path of the Extinct.Animals module to register our
    // types doesn't seem very straightformward. The environment variable
    // QML2_IMPORT_PATH is available in the test environment.
    QStringList pathList = QString(getenv("QML2_IMPORT_PATH")).split(':', Qt::SkipEmptyParts);
    for (const QString &path : pathList) {
        QString modulePath = path + "/Extinct/Animals";
        if (QFile::exists(modulePath + "/qmldir")) {
            QFileInfo moduleInfo(modulePath);
            base = QUrl::fromLocalFile(moduleInfo.absoluteFilePath()).toString();
            break;
        }
    }
#else
    base = baseUrl().toString();
#endif
    Q_ASSERT_X(!base.isEmpty(),
        "ExtinctAnimalsPlugin::registerTypes",
        "Could not find the import path containing the Extinct.Animals module");

#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    qmlRegisterType<EATarpan>();
#else
    qmlRegisterAnonymousType<EATarpan>(uri, 1);
#endif
    qmlRegisterType<EASmilodon>(uri, 1, 0, "Smilodon");
    qmlRegisterType<EASmilodon>(uri, 0, 1, "ScimitarCat");

    qmlRegisterType(QUrl(base + "/Andrewsarchus.qml"), uri, 0, 1, "Andrewsarchus");
    qmlRegisterType(QUrl(base + "/Paratriisodon.qml"), uri, 2, 0, "Andrewsarchus");

    qmlRegisterType(QUrl("qrc:///ea/Paratriisodon.qml"), uri, 2, 3, "AndrewsarchusMongoliensis");
    qmlRegisterType(QUrl("qrc:///ea/Paratriisodon.qml"), uri, 2, 4, "AndrewsarchusMongoliensis");

    qmlRegisterSingletonType<EAPangaea>(uri, 1, 0, "Pangaea", registerPangaea);
    qmlRegisterSingletonType<EAPangaea4>(uri, 4, 0, "Pangaea", registerPangaea4);

    qmlRegisterUncreatableType<EAEurope>(uri, 4, 1, "Europe", "");
    qmlRegisterUncreatableType<EAEurope42, 1>(uri, 4, 2, "Europe", "");

    qmlRegisterExtendedType<QQuickImageBase, UCQQuickImageExtension>(uri, 3, 3, "QQuickImageBase");
}
