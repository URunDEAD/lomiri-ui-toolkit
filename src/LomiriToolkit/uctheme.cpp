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

#include "uctheme_p.h"

#include <optional>
#include <utility>

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QLibraryInfo>
#include <QtCore/QStandardPaths>
#include <QtCore/QTextStream>
#include <QtGui/QFont>
#include <QtGui/QGuiApplication>
#include <QtQml/QtQml>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlInfo>
#include <QtQml/private/qqmlproperty_p.h>
#include <QtQml/private/qqmlabstractbinding_p.h>
#define foreach Q_FOREACH
#include <QtQml/private/qqmlbinding_p.h>
#undef foreach

#include "i18n_p.h"
#include "listener_p.h"
#include "quickutils_p.h"
#include "lomiritoolkitglobal.h"
#include "ucfontutils_p.h"
#include "ucstyleditembase_p_p.h"
#include "ucthemingextension_p.h"

UT_NAMESPACE_BEGIN

/*!
 * \qmltype ThemeSettings
 * \instantiates UCTheme
 * \inqmlmodule Lomiri.Components
 * \since Lomiri.Components 1.3
 * \ingroup theming
 * \brief The ThemeSettings class provides facilities to define the theme of a
 * StyledItem.
 *
 * A global instance is exposed as the \b theme context property.
 *
 * The theme defines the visual aspect of the Lomiri components. An application
 * can use one or more theme the same time. The ThemeSettings component provides
 * abilities to change the theme used by the component and all its child components.
 *
 * Changing the theme of the entire application can be achieved by changing
 * the name of the root StyledItem's, i.e. MainView's current theme.
 *
 * \qml
 * import QtQuick 2.4
 * import Lomiri.Components 1.3
 *
 * MainView {
 *     width: units.gu(40)
 *     height: units.gu(71)
 *
 *     theme.name: "Lomiri.Components.Themes.Ambiance"
 * }
 * \endqml
 * By default, styled items inherit the theme they use from their closest styled
 * item ancestor. In case the application uses MainView, all components will inherit
 * the theme from the MainView.
 * \qml
 * import QtQuick 2.4
 * import Lomiri.Components 1.3
 *
 * MainView {
 *     width: units.gu(40)
 *     height: units.gu(71)
 *
 *     Page {
 *         title: "Style test"
 *         Button {
 *             text: theme.name == "Lomiri.Components.Themes.Ambiance" ?
 *                      "SuruDark" : "Ambiance"
 *             onClicked: theme.name = (text == "Ambiance" ?
 *                      "Lomiri.Components.Themes.SuruDark" :
 *                      "Lomiri.Components.Themes.Ambiance")
 *         }
 *     }
 * }
 * \endqml
 * \note In the example above the Button inherits the theme from Page, which
 * inherits it from MainView. Therefore changing the theme name in this way will
 * result in a change of the inherited theme. In case a different theme is desired,
 * a new instance of the ThemeSettings must be created on the styled item desired.
 * \qml
 * import QtQuick 2.4
 * import Lomiri.Components 1.3
 *
 * MainView {
 *     width: units.gu(40)
 *     height: units.gu(71)
 *
 *     Page {
 *         title: "Style test"
 *         theme: ThemeSettings{}
 *         Button {
 *             text: theme.name == "Lomiri.Components.Themes.Ambiance" ?
 *                      "SuruDark" : "Ambiance"
 *             onClicked: theme.name = (text == "Ambiance" ?
 *                      "Lomiri.Components.Themes.SuruDark" :
 *                      "Lomiri.Components.Themes.Ambiance")
 *         }
 *     }
 * }
 * \endqml
 *
 * The style can be set on a StyledItem either using \l StyledItem::styleName or
 * \l StyledItem::style properties. When set through \l StyledItem::styleName,
 * the component will load the style from the current theme set, and must be a
 * QML document. The \l StyledItem::style property is a Component which can be
 * declared local, or loaded with a Loader or created using Qt.createComponent()
 * function.
 * The following example will create the style with the inherited theme.
 * \qml
 * import QtQuick 2.4
 * import Lomiri.Components 1.3
 * StyledItem {
 *     styleName: "MyItemStyle"
 * }
 * \endqml
 * All styled toolkit components such as \l Button, \l CheckBox, \l Switch, etc.
 * create their style in this way. Note that the style component must be part
 * of the theme, otherwise the style creation will fail.
 *
 * \sa {StyledItem}
 */

static const QString contextTheme = QStringLiteral("theme");
static const QString parentThemeFile = QStringLiteral("parent_theme");

quint16 UCTheme::previousVersion = 0;

#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
static inline void updateBinding(const QQmlAbstractBinding::Ptr &binding)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    if (binding->kind() == QQmlAbstractBinding::ValueTypeProxy)
#else
    if (binding->isValueTypeProxy())
#endif
        return;
    static_cast<QQmlBinding*>(binding.data())->update();
}
#else
static inline void updateBinding (const QQmlAbstractBinding *binding)
{
    const_cast<QQmlAbstractBinding*>(binding)->update();
}
#endif

static QStringList themeSearchPath()
{
    QString envPath = QLatin1String(getenv("LOMIRI_UI_TOOLKIT_THEMES_PATH"));
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    QStringList pathList = envPath.split(':', QString::SkipEmptyParts);
#else
    QStringList pathList = envPath.split(':', Qt::SkipEmptyParts);
#endif
    if (pathList.isEmpty()) {
        // get the default path list from generic data location, which contains
        // XDG_DATA_DIRS
        QString xdgDirs = QLatin1String(getenv("XDG_DATA_DIRS"));
        if (!xdgDirs.isEmpty()) {
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
            pathList << xdgDirs.split(':', QString::SkipEmptyParts);
#else
            pathList << xdgDirs.split(':', Qt::SkipEmptyParts);
#endif
        }
        // ~/.local/share
        pathList << QStandardPaths::standardLocations(QStandardPaths::GenericDataLocation);
    }

    // append QML import path(s); we must explicitly support env override here
    const QString qml2ImportPath = QString::fromLocal8Bit(getenv("QML2_IMPORT_PATH"));
    if (!qml2ImportPath.isEmpty()) {
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
        pathList << qml2ImportPath.split(':', QString::SkipEmptyParts);
#else
        pathList << qml2ImportPath.split(':', Qt::SkipEmptyParts);
#endif
    }
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    pathList << QLibraryInfo::location(QLibraryInfo::Qml2ImportsPath).split(':', QString::SkipEmptyParts);
#elif QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    pathList << QLibraryInfo::location(QLibraryInfo::Qml2ImportsPath).split(':', Qt::SkipEmptyParts);
#else
    pathList << QLibraryInfo::paths(QLibraryInfo::Qml2ImportsPath);
#endif
    // fix folders
    QStringList result;
    Q_FOREACH(const QString &path, pathList) {
        QDir dir(path);
        if (dir.exists()) {
            result << dir.absolutePath();
        }
    }
    // prepend current folder
    result.prepend(QDir::currentPath());
    return result;
}

static QString parentThemeName(const UCTheme::ThemeRecord& themePath)
{
    QString parentTheme;
    QFile file(themePath.path.filePath(parentThemeFile));
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        parentTheme = in.readLine();
    }
    return parentTheme;
}

static QUrl fileToUrl(const QFile &file) {
    if (file.fileName().startsWith(":")) {
        return QUrl("qrc" + file.fileName());
    } else {
        return QUrl::fromLocalFile(file.fileName());
    }
}

/******************************************************************************
 * Theme::PaletteConfig
 */

// builds configuration list and applies the configuration on the palette
void UCTheme::PaletteConfig::configurePalette(QObject *themePalette)
{
    if (!palette || !themePalette || configured) {
        return;
    }
    if (configList.isEmpty()) {
        // need to build config list first
        buildConfig();
    }
    if (!configList.isEmpty()) {
        apply(themePalette);
    }
}

void UCTheme::PaletteConfig::restorePalette()
{
    if (!palette || configList.isEmpty() || !configured) {
        return;
    }

    for (int i = 0; i < configList.count(); i++) {
        Data &config = configList[i];
        if (!config.paletteProperty.isValid()) {
            continue;
        }

        // restore the config binding to the config target
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        if (config.configBinding && config.configBinding->kind() == QQmlAbstractBinding::QmlBinding) {
#else
        if (config.configBinding && !config.configBinding->isValueTypeProxy()) {
#endif
            QQmlBinding *qmlBinding = static_cast<QQmlBinding*>(config.configBinding.data());
            qmlBinding->removeFromObject();
            QQmlPropertyPrivate::removeBinding(config.paletteProperty);
            qmlBinding->setTarget(config.configProperty);
        }

        if (config.paletteBinding) {
            // restore the binding to the palette
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
            QQmlAbstractBinding::Ptr prev(QQmlPropertyPrivate::binding(config.paletteProperty));
            QQmlPropertyPrivate::setBinding(config.paletteProperty, config.paletteBinding.data());
#else
            QQmlAbstractBinding *prev = QQmlPropertyPrivate::setBinding(config.paletteProperty, config.paletteBinding);
#endif
            if (prev && prev != config.paletteBinding && prev != config.configBinding) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
                prev->removeFromObject();
                prev.reset();
#else
                prev->destroy();
#endif
            }
            updateBinding(config.paletteBinding);
        } else {
            config.paletteProperty.write(config.paletteValue);
        }

        config.paletteProperty = QQmlProperty();
        config.paletteBinding = Q_NULLPTR;
        config.paletteValue.clear();
    }

    configured = false;
}

// build palette configuration list
void UCTheme::PaletteConfig::buildConfig()
{
    if (!palette) {
        return;
    }
    const char* valueSetString[2] = { "normal", "selected" };
    const QString valueSetQString[2] = { QStringLiteral("normal"), QStringLiteral("selected") };
    QQmlContext *configContext = qmlContext(palette);

    for (int i = 0; i < 2; i++) {
        QObject *configObject = palette->property(valueSetString[i]).value<QObject*>();
        const QMetaObject *mo = configObject->metaObject();

        for (int ii = mo->propertyOffset(); ii < mo->propertyCount(); ii++) {
            const QMetaProperty prop = mo->property(ii);
            QString propertyName = QStringLiteral("%1.%2")
                .arg(valueSetQString[i]).arg(QString::fromLatin1(prop.name()));
            QQmlProperty configProperty(palette, propertyName, configContext);

            // first we need to check whether the property has a binding or not
            QQmlAbstractBinding *binding = QQmlPropertyPrivate::binding(configProperty);
            if (binding) {
                configList << Data(propertyName, configProperty, binding);
            } else {
                QVariant value = configProperty.read();
                QColor color = value.value<QColor>();
                if (color.isValid()) {
                    configList << Data(propertyName, configProperty);
                }
            }
        }
    }
}

// apply configuration on the palette
void UCTheme::PaletteConfig::apply(QObject *themePalette)
{
    QQmlContext *context = qmlContext(themePalette);
    for (int i = 0; i < configList.count(); i++) {
        Data &config = configList[i];
        config.paletteProperty = QQmlProperty(themePalette, config.propertyName, context);

        // backup
        config.paletteBinding = QQmlPropertyPrivate::binding(config.paletteProperty);
        if (!config.paletteBinding) {
            config.paletteValue = config.paletteProperty.read();
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
        } else {
            QQmlPropertyPrivate::removeBinding(config.paletteProperty);
#endif
        }

        // apply configuration
        if (config.configBinding) {
            // transfer binding's target
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
            if (config.configBinding->kind() == QQmlAbstractBinding::QmlBinding) {
                QQmlBinding *qmlBinding = static_cast<QQmlBinding*>(config.configBinding.data());
#else
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
            if (!config.configBinding->isValueTypeProxy()) {
                QQmlBinding *qmlBinding = static_cast<QQmlBinding*>(config.configBinding.data());
#else
            if (config.configBinding->bindingType() == QQmlAbstractBinding::Binding) {
                QQmlBinding *qmlBinding = static_cast<QQmlBinding*>(config.configBinding);
#endif
#endif
                qmlBinding->setTarget(config.paletteProperty);
            }
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
            QQmlPropertyPrivate::setBinding(config.paletteProperty, config.configBinding.data());
#else
            QQmlPropertyPrivate::setBinding(config.paletteProperty, config.configBinding);
#endif
        } else {
            if (config.paletteBinding) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
                QQmlPropertyPrivate::removeBinding(config.paletteProperty);
#else
                // remove binding so the property doesn't clear it
                QQmlPropertyPrivate::setBinding(config.paletteProperty, 0);
#endif
            }
            config.paletteProperty.write(config.configProperty.read());
        }
    }
    configured = true;
}

/******************************************************************************
 * Theme
 */
UCTheme::UCTheme(QObject *parent)
    : QObject(parent)
    , m_parentTheme(Q_NULLPTR)
    , m_palette(Q_NULLPTR)
{
    QObject::connect(&m_defaultTheme, &UCDefaultTheme::themeNameChanged,
                     this, &UCTheme::_q_defaultThemeChanged);
}

UCTheme *UCTheme::defaultTheme(QQmlEngine *engine)
{
    if (!engine || !engine->rootContext()) {
        return Q_NULLPTR;
    }

    QQmlContext *context = engine->rootContext();
    UCTheme *theme = context->contextProperty(contextTheme).value<UCTheme*>();
    if (!theme) {
        theme = new UCTheme(engine);
        theme->setupDefault();
        QQmlEngine::setContextForObject(theme, context);
        context->setContextProperty(contextTheme, theme);

        ContextPropertyChangeListener *listener =
            new ContextPropertyChangeListener(context, contextTheme);
        QObject::connect(theme, &UCTheme::nameChanged,
                        listener, &ContextPropertyChangeListener::updateContextProperty);
    }
    return theme;
}

void UCTheme::setupDefault()
{
    // FIXME: move this into QPA
    // set the default font
    QFont defaultFont = QGuiApplication::font();
    defaultFont.setFamily(QStringLiteral("Ubuntu"));
    defaultFont.setPixelSize(UCFontUtils::instance()->sizeToPixels(QStringLiteral("medium")));
    defaultFont.setWeight(QFont::Light);
    QGuiApplication::setFont(defaultFont);
    setObjectName(QStringLiteral("default"));
}

void UCTheme::updateEnginePaths()
{
    QQmlEngine *engine = qmlEngine(this);
    if (!engine) {
        qWarning() << "UCTheme::updateEnginePaths() called without valid engine";
        return;
    }

    // To preserve the paths search order, iterate over them
    // in reverse, as engine->addImportPath prepends them.
    QStringList paths = themeSearchPath();
    for (auto it = paths.crbegin(); it != paths.crend(); it++) {
        if (QDir(*it).exists() && !engine->importPathList().contains(*it)) {
            engine->addImportPath(*it);
        }
    }
}

void UCTheme::_q_defaultThemeChanged()
{
    m_themePaths.reset();
    loadPalette(qmlEngine(this));
    Q_EMIT nameChanged();
    updateThemedItems();
}

std::optional<UCTheme::ThemeRecord> UCTheme::pathFromThemeName(const QString &themeName)
{
    if (themeName.isEmpty() || !qmlEngine(this)) {
        return std::nullopt;
    }
    QString themePathPart = themeName;
    themePathPart.replace('.', '/');

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    // Attempt to load the qml module that contains the theme.
    // We need this to be able to access its qrc paths later.
    // The module is loaded even if it doesn't contain a Palette.qml.
    QQmlComponent component(qmlEngine(this));
    component.loadFromModule(themeName, "Palette");
#endif

    // Search for the module in the QML engine's import paths.
    // If the Qt resource system is used, the file system path will just
    // contain the .so files and actual sources are embedded.
    // Otherwise, the files exist on the file system.
    std::optional<QDir> qrcModulePath;
    std::optional<QDir> fsModulePath;
    for (const QString &path : qmlEngine(this)->importPathList()) {
        bool isQrc = path.startsWith("qrc:/");
        QDir importPath((isQrc ? ":" : "") + QUrl(path).path());
        QDir modulePath = importPath.filePath(themePathPart);
        if (isQrc && !qrcModulePath && modulePath.exists()) {
            qrcModulePath = modulePath;
        } else if(!isQrc && !fsModulePath && modulePath.exists()) {
            fsModulePath = modulePath;
        }
    }

    // Consider the qrc path first
    std::optional<QDir> &modulePath = qrcModulePath ? qrcModulePath : fsModulePath;
    if (modulePath) {
        return UCTheme::ThemeRecord(
            themeName,
            *modulePath,
            QFile::exists(modulePath->filePath("qmldir")),
            QFile::exists(modulePath->filePath("deprecated"))
        );
    }
    qWarning() << qPrintable(QStringLiteral("Theme not found: \"%1\"").arg(themeName));
    return std::nullopt;
}

void UCTheme::updateThemePaths()
{
    m_themePaths.emplace();
    std::optional<UCTheme::ThemeRecord> themePath = pathFromThemeName(name());;
    while (themePath) {
        m_themePaths->append(*themePath);
        themePath = pathFromThemeName(parentThemeName(*themePath));
    }
}

QList<UCTheme::ThemeRecord>& UCTheme::themePaths() {
    if (!m_themePaths) {
        updateEnginePaths();
        updateThemePaths();
    }
    return *m_themePaths;
}

/*!
 * \qmlproperty ThemeSettings ThemeSettings::parentTheme
 * \readonly
 * The property specifies the parent ThemeSettings instance.
 */
UCTheme *UCTheme::parentTheme()
{
    return  m_parentTheme.data();
}

void UCTheme::setParentTheme(UCTheme *parentTheme)
{
    if (m_parentTheme == parentTheme || parentTheme == this) {
        return;
    }
    Q_ASSERT(parentTheme);
    m_parentTheme = parentTheme;
    Q_EMIT parentThemeChanged();
}

/*!
 * \qmlproperty string ThemeSettings::name
 * The name of the current theme in dotted format i.e. "Lomiri.Components.Themes.Ambiance".
 */
QString UCTheme::name() const
{
    return !m_name.isEmpty() ? m_name : m_defaultTheme.themeName();
}
void UCTheme::setName(const QString& name)
{
    if (name == m_name) {
        return;
    }
    m_name = name;
    m_themePaths.reset();
    if (name.isEmpty()) {
        QObject::connect(&m_defaultTheme, &UCDefaultTheme::themeNameChanged,
                         this, &UCTheme::_q_defaultThemeChanged);
    } else {
        QObject::disconnect(&m_defaultTheme, &UCDefaultTheme::themeNameChanged,
                            this, &UCTheme::_q_defaultThemeChanged);
    }
    loadPalette(qmlEngine(this));
    Q_EMIT nameChanged();
    updateThemedItems();
}
void UCTheme::resetName()
{
    setName(QString());
}

/*!
 * \qmlproperty Palette ThemeSettings::palette
 * The palette of the current theme. When set, only the valid palette values will
 * be taken into account, which will override the theme defined palette values.
 * The following example will set the system's default theme palette normal background
 * color to Lomiri blue. All other palette values will be untouched.
 * \qml
 * import QtQuick 2.4
 * import Lomiri.Components 1.3
 * import Lomiri.Components.Themes 1.0
 *
 * MainView {
 *     // your code
 *     theme.palette: Palette {
 *         normal.background: LomiriColors.blue
 *     }
 * }
 * \endqml
 * \note Palette values applied on inherited themes will be rolled back once the
 * component declaring the palette is unloaded. This can be demonstracted using
 * a Loader element:
 * \qml
 * import QtQuick 2.4
 * import Lomiri.Components 1.3
 * import Lomiri.Components.Themes 1.0
 *
 * MainView {
 *     width: units.gu(40)
 *     height: units.gu(71)
 *
 *     Loader {
 *         id: loader
 *         onItemChanged: if (item) button.theme.palette = item
 *     }
 *     Component {
 *         id: dynamicPalette
 *         Palette {
 *             normal.background: LomiriColors.blue
 *         }
 *     }
 *     Button {
 *         id: button
 *         text: "Toggle palette"
 *         onClicked: {
 *             if (loader.item) {
 *                 loader.sourceComponent = undefined;
 *             } else {
 *                 loader.sourceComponent = dynamicPalette;
 *             }
 *         }
 *     }
 * }
 * \endqml
 * The palette doesn't need to be reset as it automatically resets when the
 * palette used for configuration is destroyed.
 */
QObject* UCTheme::palette(quint16 version)
{
    if (!m_palette) {
        if (version) {
            // force version to be used
            previousVersion = version;
        }
        loadPalette(qmlEngine(this), false);
    }
    return m_palette;
}
void UCTheme::setPalette(QObject *config)
{
    if (config == palette() || config == m_config.palette) {
        return;
    }
    if (config && !QuickUtils::inherits(config, QStringLiteral("Palette"))) {
        qmlWarning(config) << QStringLiteral("Not a Palette component.");
        return;
    }

    // 1. restore original palette values
    m_config.restorePalette();
    // 2. clear config list
    m_config.reset();
    // disconnect the reset from the previous palette
    if (m_config.palette) {
        disconnect(m_config.palette, &QObject::destroyed,
                   this, 0);
    }
    // 3. apply palette configuration
    m_config.palette = config;
    if (m_config.palette) {
        connect(m_config.palette, &QObject::destroyed,
                this, &UCTheme::resetPalette,
                Qt::DirectConnection);
        m_config.configurePalette(palette());
    }
    Q_EMIT paletteChanged();
}
void UCTheme::resetPalette()
{
    setPalette(NULL);
}

QUrl UCTheme::styleUrl(const QString& styleName, quint16 version, bool *isFallback)
{
    if (isFallback) {
        (*isFallback) = false;
    }

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    // For Qt6 only register 2.0 version for now.
    for (const ThemeRecord &themePath : themePaths()) {
        const quint16 major = 2;
        const quint16 minor = 0;
        QString versionedName = QStringLiteral("%1.%2/%3").arg(major).arg(minor).arg(styleName);
        QFile styleFile(themePath.path.filePath(versionedName));
        if (styleFile.exists()) {
            return fileToUrl(styleFile);
        }
    }
#else
    // loop through the versions first, so we will look after the style in all
    // the parents, then fall back to the older version
    quint16 major = MAJOR_VERSION(version);
    // loop through the versions to see if we have one matching
    // we stop at version 1.2 as we do not have support for earlier themes anymore.
    for (int minor = MINOR_VERSION(version); minor >= 2; minor--) {
        // check with each path of the theme
        for (const ThemeRecord &themePath : themePaths()) {
            /*
             * There are two cases where we have to deal with non-versioned styles: application
             * themes made for the previous theming and deprecated themes. For shared themes,
             * we have to check the fallback case.
             */
            quint16 styleVersion = BUILD_VERSION(major, minor);
            if (themePath.deprecated) {
                styleVersion = 0;
            }
            if (themePath.shared && (minor < 2)) {
                styleVersion = LATEST_UITK_VERSION;
            }
            QString versionedName = QStringLiteral("%1.%2/%3").arg(major).arg(minor).arg(styleName);
            QFile styleFile(themePath.path.filePath(versionedName));
            if (styleFile.exists()) {
                // set fallback warning if the theme is shared
                if (isFallback && themePath.shared && (version != styleVersion)) {
                    (*isFallback) = true;
                }
                return fileToUrl(styleFile);
            }

            // if we don't get any style, get the non-versioned ones for non-shared and deprecated styles
            if (!themePath.shared || themePath.deprecated) {
                QFile styleFile(themePath.path.filePath(styleName));
                if (styleFile.exists()) {
                    return fileToUrl(styleFile);
                }
            }
        }
    }
#endif

    return QUrl();
}

void UCTheme::attachItem(QQuickItem *item, bool attach)
{
    if (attach) {
        m_attachedItems.append(item);
    } else {
        m_attachedItems.removeOne(item);
    }
}

void UCTheme::updateThemedItems()
{
    for (int i = 0; i < m_attachedItems.count(); i++) {
        UCThemingExtension *extension = qobject_cast<UCThemingExtension*>(m_attachedItems[i]);
        if (extension) {
            extension->itemThemeReloaded(this);
        }
    }
}

/*
 * Updates the version used by the toolkit/application
 */
void UCTheme::checkMixedVersionImports(QQuickItem *item, quint16 version)
{
    static bool wasShown = false;
    if (version != previousVersion && previousVersion && !wasShown) {
        // the first change is due to the first import detection, any further changes would mean there are
        // multiple version imports
        QString msg = QStringLiteral("Mixing of Lomiri.Components module versions %1.%2 and %3.%4 detected!")
                .arg(MAJOR_VERSION(version))
                .arg(MINOR_VERSION(version))
                .arg(MAJOR_VERSION(previousVersion))
                .arg(MINOR_VERSION(previousVersion));
        qmlWarning(item) << msg;
        wasShown = true;
    }
    previousVersion = version;
}

/*
 * Returns an instance of the style component named \a styleName and parented
 * to \a parent.
 */
QQmlComponent* UCTheme::createStyleComponent(const QString& styleName, QObject* parent, quint16 version)
{
    QQmlComponent *component = NULL;
    Q_ASSERT(version);

    if (parent != NULL) {
        QQmlEngine* engine = qmlEngine(parent);
        if (!engine) {
            // we may be in the phase when the qml context is not yet defined for the parent
            // so for now we return NULL
            return Q_NULLPTR;
        }
        // make sure we have the paths
        bool fallback = false;
        QUrl url = styleUrl(styleName, version, &fallback);
        if (url.isValid()) {
            if (fallback) {
                qmlWarning(parent) << QStringLiteral("Theme '%1' has no '%2' style for version %3.%4, fall back to version %5.%6.")
                                   .arg(name()).arg(styleName).arg(MAJOR_VERSION(version)).arg(MINOR_VERSION(version))
                                   .arg(MAJOR_VERSION(LATEST_UITK_VERSION)).arg(MINOR_VERSION(LATEST_UITK_VERSION));
            }
            component = new QQmlComponent(engine, url, QQmlComponent::PreferSynchronous, parent);
            if (component->isError()) {
                qmlWarning(parent) << component->errorString();
                delete component;
                component = NULL;
            } else {
                // set context for the component
                QQmlEngine::setContextForObject(component, qmlContext(parent));
            }
        } else {
            qmlWarning(parent) <<
               QStringLiteral("Warning: Style %1 not found in theme %2").arg(styleName).arg(name());
        }
    }

    return component;
}

void UCTheme::loadPalette(QQmlEngine *engine, bool notify)
{
    if (!engine) {
        return;
    }
    if (m_palette) {
        // restore bindings to the config palette before we delete
        m_config.restorePalette();
        delete m_palette;
        m_palette = 0;
    }
    // theme may not have palette defined
    QUrl paletteUrl = styleUrl(
        QStringLiteral("Palette.qml"), previousVersion ? previousVersion : LATEST_UITK_VERSION);
    if (paletteUrl.isValid()) {
        m_palette = QuickUtils::instance()->createQmlObject(paletteUrl, engine);
        if (m_palette) {
            m_palette->setParent(this);
        }
        m_config.configurePalette(m_palette);
        if (notify) {
            Q_EMIT paletteChanged();
        }
    } else {
        // use the default palette if none defined
        UCTheme *defTheme = defaultTheme(engine);
        if (this == defTheme) {
            QString msg("Warning: Default theme %1 doesn't provide Palette.qml");
            qWarning() << qPrintable(msg.arg(defTheme->name()));
        } else {
            m_palette = defTheme->palette();
        }
    }
}

// returns the palette color value of a color profile
QColor UCTheme::getPaletteColor(const char *profile, const char *color)
{
    QColor result;
    QObject *pal = palette();
    if (pal) {
        QObject *paletteProfile = pal->property(profile).value<QObject*>();
        if (paletteProfile) {
            result = paletteProfile->property(color).value<QColor>();
        }
    }
    return result;
}

UT_NAMESPACE_END
