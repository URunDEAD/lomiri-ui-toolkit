/*
 * Copyright 2016 Canonical Ltd.
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
 * Authors: Zsombor Egri <zsombor.egri@canonical.com>
 *          Florian Boucault <florian.boucault@canonical.com>
 */

#ifndef UCTHEME_P_H
#define UCTHEME_P_H

#include <optional>

#include <QtCore/QDir>
#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtCore/QString>
#include <QtQml/QQmlComponent>
#include <QtQml/QQmlProperty>
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
#include <QtQml/private/qqmlabstractbinding_p.h>
#endif
#include <QtQml/private/qpodvector_p.h>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <LomiriToolkit-Qt6/lomiritoolkitglobal.h>
#else
#include <LomiriToolkit/lomiritoolkitglobal.h>
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <LomiriToolkit-Qt6/private/ucdefaulttheme_p.h>
#else
#include <LomiriToolkit/private/ucdefaulttheme_p.h>
#endif

#if QT_VERSION < QT_VERSION_CHECK(5, 6, 0)
class QQmlAbstractBinding;
#endif
class QQuickItem;

UT_NAMESPACE_BEGIN

class UCStyledItemBase;
class LOMIRITOOLKIT_EXPORT UCTheme : public QObject
{
    Q_OBJECT
    Q_PROPERTY(UT_PREPEND_NAMESPACE(UCTheme) *parentTheme READ parentTheme NOTIFY parentThemeChanged FINAL)
    Q_PROPERTY(QString name READ name WRITE setName RESET resetName NOTIFY nameChanged FINAL)
    Q_PROPERTY(QObject* palette READ palette WRITE setPalette RESET resetPalette NOTIFY paletteChanged FINAL)
public:

    static quint16 previousVersion;
    struct ThemeRecord {
        ThemeRecord() :
            shared(false), deprecated(false)
        {}
        ThemeRecord(const QString &name, const QDir &path, bool shared, bool deprecated) :
            name(name), path(path), shared(shared), deprecated(deprecated)
        {}

        QString name;
        QDir path;
        bool shared:1;
        bool deprecated:1;
    };

    explicit UCTheme(QObject *parent = 0);
    static UCTheme *defaultTheme(QQmlEngine *engine);
    static std::optional<UCTheme::ThemeRecord> pathFromThemeName(QQmlEngine *engine, const QString &themeName);

    // getter/setters
    UCTheme *parentTheme();
    void setParentTheme(UCTheme *parentTheme); // not used as setter
    QString name() const;
    void setName(const QString& name);
    void resetName();
    QObject* palette(quint16 version = 0);
    void setPalette(QObject *config);
    static void checkMixedVersionImports(QQuickItem *item, quint16 version);

    // internal, used by the deprecated Theme.createStyledComponent()
    QQmlComponent* createStyleComponent(const QString& styleName, QObject* parent, quint16 version = 0);
    void attachItem(QQuickItem *item, bool attach);

    // helper functions
    QColor getPaletteColor(const char *profile, const char *color);

Q_SIGNALS:
    void parentThemeChanged();
    void nameChanged();
    void paletteChanged();
    void versionChanged();

private Q_SLOTS:
    void resetPalette();
    void _q_defaultThemeChanged();

private:
    void setupDefault();
    void updateEnginePaths();
    void updateThemePaths();
    QList<ThemeRecord>& themePaths();
    QUrl styleUrl(const QString& styleName, quint16 version, bool *isFallback = NULL);
    void loadPalette(QQmlEngine *engine, bool notify = true);
    void updateThemedItems();

    class PaletteConfig
    {
    public:
        PaletteConfig()
            : palette(0), configured(false)
        {}
        ~PaletteConfig()
        {
            restorePalette();
        }

        void configurePalette(QObject *themePalette);
        void restorePalette();
        void reset()
        {
            configList.clear();
        }

        QObject *palette;
    private:
        void buildConfig();
        void apply(QObject *palette);

        struct Data {
            Data(const QString &name, const QQmlProperty &prop)
                : propertyName(name), configProperty(prop), configBinding(0), paletteBinding(0)
            {}
            Data(const QString &name, const QQmlProperty &prop, QQmlAbstractBinding *binding)
                : propertyName(name), configProperty(prop), configBinding(binding), paletteBinding(0)
            {}

            QString propertyName;
            QQmlProperty configProperty;
            QQmlProperty paletteProperty;
            QVariant paletteValue;
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
            QQmlAbstractBinding::Ptr configBinding;
            QQmlAbstractBinding::Ptr paletteBinding;
#else
            QQmlAbstractBinding *configBinding;
            QQmlAbstractBinding *paletteBinding;
#endif
        };

        // configuration palette, not the original theme one
        bool configured:1;
        QList<Data> configList;
    };

    PaletteConfig m_config;
    QString m_name;
    QPointer<UCTheme> m_parentTheme;
    QPointer<QObject> m_palette; // the palette might be from the default style if the theme doesn't define palette
    std::optional<QList<ThemeRecord>> m_themePaths;
    UCDefaultTheme m_defaultTheme;
    QPODVector<QQuickItem*, 4> m_attachedItems;

    friend class UCDeprecatedTheme;
};

UT_NAMESPACE_END

QML_DECLARE_TYPE(UT_PREPEND_NAMESPACE(UCTheme))

#endif // UCTHEME_P_H
