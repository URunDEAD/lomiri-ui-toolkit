/*
 * Copyright 2013-2016 Canonical Ltd.
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
 */

#include <QtCore/QString>
#include <QtQml/QQmlEngine>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlComponent>
#include <QtQuick/private/qquicktext_p.h>
#include <QtTest/QtTest>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <LomiriToolkit-Qt6/lomiritoolkitmodule.h>
#else
#include <LomiriToolkit/lomiritoolkitmodule.h>
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <LomiriToolkit-Qt6/private/ucdeprecatedtheme_p.h>
#else
#include <LomiriToolkit/private/ucdeprecatedtheme_p.h>
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <LomiriToolkit-Qt6/private/uctheme_p.h>
#else
#include <LomiriToolkit/private/uctheme_p.h>
#endif

#include "uctestcase.h"

UT_USE_NAMESPACE

Q_DECLARE_METATYPE(QList<QQmlError>)

class ThemeTestCase : public LomiriTestCase
{
    Q_OBJECT
public:
    ThemeTestCase(const QString& file, QWindow* parent = 0)
        : LomiriTestCase(file, QQuickView::SizeViewToRootObject, true, parent)
    {
    }

    ~ThemeTestCase()
    {
        // restore theme before quitting
        if (!rootContext()) {
            return;
        }
        UCDeprecatedTheme *theme = rootContext()->contextProperty("Theme").value<UCDeprecatedTheme*>();
        if (theme) {
            theme->resetName();
        } else {
            qWarning() << "No theme instance found!";
        }
    }

    void setTheme(const QString &theme)
    {
        rootObject()->setProperty("themeName", theme);
        QCoreApplication::processEvents();
    }

    void setStyle(const QString &style)
    {
        rootObject()->setProperty("styleDocument", style);
        QCoreApplication::processEvents();
    }
};

class tst_UCDeprecatedTheme : public QObject
{
    Q_OBJECT
private:
    QString m_xdgDataPath;

    UCDeprecatedTheme *instance(QQmlEngine &engine)
    {
        return engine.rootContext()->contextProperty("Theme").value<UCDeprecatedTheme*>();
    }
    void initDeprecatedTheme(QQmlEngine &engine)
    {
        LomiriToolkitModule::initializeContextProperties(&engine);
    }

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testNameDefault();
    void testNameSet();
    void testCreateStyleComponent();
    void testCreateStyleComponent_data();
    void testThemesRelativePath();
    void testThemesRelativePathWithParent();
    void testThemesRelativePathWithParentXDGDATA();
    void testThemesRelativePathWithParentNoVariablesSet();
    void testThemesRelativePathWithParentOneXDGPathSet();
    void testNoImportPathSet();
    void testBogusImportPathSet();
    void testMultipleImportPathsSet();
    void testPaletteUsed_bug1549830();
};

void tst_UCDeprecatedTheme::initTestCase()
{
    m_xdgDataPath = QLatin1String(getenv("XDG_DATA_DIRS"));
    qputenv("SUPPRESS_DEPRECATED_NOTE", "yes");
}

void tst_UCDeprecatedTheme::cleanupTestCase()
{
    qputenv("XDG_DATA_DIRS", m_xdgDataPath.toLocal8Bit());
}

void tst_UCDeprecatedTheme::testNameDefault()
{
    QQmlEngine engine;
    initDeprecatedTheme(engine);
    QCOMPARE(instance(engine)->name(), QString("Lomiri.Components.Themes.Ambiance"));
}

void tst_UCDeprecatedTheme::testNameSet()
{
    QTest::ignoreMessage(QtWarningMsg, "Theme not found: \"MyBeautifulTheme\"");
    QTest::ignoreMessage(QtWarningMsg, "Warning: Default theme MyBeautifulTheme doesn't provide Palette.qml");

    QQmlEngine engine;
    initDeprecatedTheme(engine);
    instance(engine)->setName("MyBeautifulTheme");
    QCOMPARE(instance(engine)->name(), QString("MyBeautifulTheme"));
}

void tst_UCDeprecatedTheme::testCreateStyleComponent()
{
    QFETCH(QString, styleName);
    QFETCH(QString, parentName);
    QFETCH(bool, success);

    QTest::ignoreMessage(QtWarningMsg, "Warning: Default theme TestModule.TestTheme doesn't provide Palette.qml");
    if (parentName == "SimpleItem.qml")
        QTest::ignoreMessage(QtWarningMsg, "QQmlComponent: Component is not ready");
    else if (styleName == "NotExistingTestStyle.qml")
        ThemeTestCase::ignoreWarning(parentName, 20, 1, "QML Parent: Warning: Style NotExistingTestStyle.qml not found in theme TestModule.TestTheme");

    qputenv("LOMIRI_UI_TOOLKIT_THEMES_PATH", ".");

    QScopedPointer<ThemeTestCase> view(new ThemeTestCase(parentName));
    QVERIFY(view);
    view->setTheme("TestModule.TestTheme");
    view->setStyle(styleName);
    QQmlComponent *style = view->rootObject()->property("style").value<QQmlComponent*>();
    QCOMPARE(style != NULL, success);
}

void tst_UCDeprecatedTheme::testCreateStyleComponent_data() {
    QTest::addColumn<QString>("styleName");
    QTest::addColumn<QString>("parentName");
    QTest::addColumn<bool>("success");
    QTest::newRow("Existing style") << "TestStyle.qml" << "Parent.qml" << true;
    QTest::newRow("Non existing style") << "NotExistingTestStyle.qml" << "Parent.qml" << false;
}

void tst_UCDeprecatedTheme::testThemesRelativePath()
{
    QTest::ignoreMessage(QtWarningMsg, "Warning: Default theme TestModule.TestTheme doesn't provide Palette.qml");

    qputenv("LOMIRI_UI_TOOLKIT_THEMES_PATH", "../tst_theme_engine");

    QScopedPointer<ThemeTestCase> view(new ThemeTestCase("Parent.qml"));
    QVERIFY(view);
    view->setTheme("TestModule.TestTheme");
    view->setStyle("TestStyle.qml");
    QQmlComponent *style = view->rootObject()->property("style").value<QQmlComponent*>();
    QCOMPARE(style != NULL, true);
}

void tst_UCDeprecatedTheme::testThemesRelativePathWithParent()
{
    QTest::ignoreMessage(QtWarningMsg, "Warning: Default theme CustomTheme doesn't provide Palette.qml");

    qputenv("LOMIRI_UI_TOOLKIT_THEMES_PATH", "./themes:./themes/TestModule");

    QScopedPointer<ThemeTestCase> view(new ThemeTestCase("Parent.qml"));
    QVERIFY(view);
    view->setTheme("CustomTheme");
    view->setStyle("TestStyle.qml");
    QQmlComponent *style = view->rootObject()->property("style").value<QQmlComponent*>();
    QCOMPARE(style != NULL, true);
}

void tst_UCDeprecatedTheme::testThemesRelativePathWithParentXDGDATA()
{
    QTest::ignoreMessage(QtWarningMsg, "Warning: Default theme CustomTheme doesn't provide Palette.qml");

    qputenv("LOMIRI_UI_TOOLKIT_THEMES_PATH", "");
    qputenv("XDG_DATA_DIRS", "./themes:./themes/TestModule");

    QScopedPointer<ThemeTestCase> view(new ThemeTestCase("Parent.qml"));
    QVERIFY(view);
    view->setTheme("CustomTheme");
    view->setStyle("TestStyle.qml");
    QQmlComponent *style = view->rootObject()->property("style").value<QQmlComponent*>();
    QCOMPARE(style != NULL, true);
}

void tst_UCDeprecatedTheme::testThemesRelativePathWithParentNoVariablesSet()
{
    ThemeTestCase::ignoreWarning("Parent.qml", 20, 1, "QML Parent: Warning: Style TestStyle.qml not found in theme Lomiri.Components.Themes.Ambiance");

    qputenv("LOMIRI_UI_TOOLKIT_THEMES_PATH", "");
    qputenv("XDG_DATA_DIRS", "");

    QScopedPointer<ThemeTestCase> view(new ThemeTestCase("Parent.qml"));
    QVERIFY(view);
    view->setStyle("TestStyle.qml");
}

void tst_UCDeprecatedTheme::testThemesRelativePathWithParentOneXDGPathSet()
{
    QTest::ignoreMessage(QtWarningMsg, "Warning: Default theme TestModule.TestTheme doesn't provide Palette.qml");

    qputenv("LOMIRI_UI_TOOLKIT_THEMES_PATH", "");
    qputenv("XDG_DATA_DIRS", "../tst_theme_engine");

    QScopedPointer<ThemeTestCase> view(new ThemeTestCase("Parent.qml"));
    QVERIFY(view);
    view->setTheme("TestModule.TestTheme");
    view->setStyle("TestStyle.qml");
    QQmlComponent *style = view->rootObject()->property("style").value<QQmlComponent*>();
    QCOMPARE(style != NULL, true);
}

void tst_UCDeprecatedTheme::testNoImportPathSet()
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    if (!QFile(QLibraryInfo::path(QLibraryInfo::Qml2ImportsPath) + "/Lomiri/Components").exists())
#else
    if (!QFile(QLibraryInfo::location(QLibraryInfo::Qml2ImportsPath) + "/Lomiri/Components").exists())
#endif
        QSKIP("This can only be tested if the UITK is installed");

    qputenv("LOMIRI_UI_TOOLKIT_THEMES_PATH", "");
    qputenv("XDG_DATA_DIRS", "");
    qputenv("QML2_IMPORT_PATH", "");

    QQmlEngine engine;
    initDeprecatedTheme(engine);
    QCOMPARE(instance(engine)->name(), QString("Lomiri.Components.Themes.Ambiance"));
}

void tst_UCDeprecatedTheme::testBogusImportPathSet()
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    if (!QFile(QLibraryInfo::path(QLibraryInfo::Qml2ImportsPath) + "/Lomiri/Components").exists())
#else
    if (!QFile(QLibraryInfo::location(QLibraryInfo::Qml2ImportsPath) + "/Lomiri/Components").exists())
#endif
        QSKIP("This can only be tested if the UITK is installed");

    qputenv("LOMIRI_UI_TOOLKIT_THEMES_PATH", "");
    qputenv("XDG_DATA_DIRS", "");
    qputenv("QML2_IMPORT_PATH", "/no/plugins/here");

    QQmlEngine engine;
    initDeprecatedTheme(engine);
    QCOMPARE(instance(engine)->name(), QString("Lomiri.Components.Themes.Ambiance"));
}

void tst_UCDeprecatedTheme::testMultipleImportPathsSet()
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    if (!QFile(QLibraryInfo::path(QLibraryInfo::Qml2ImportsPath) + "/Lomiri/Components").exists())
#else
    if (!QFile(QLibraryInfo::location(QLibraryInfo::Qml2ImportsPath) + "/Lomiri/Components").exists())
#endif
        QSKIP("This can only be tested if the UITK is installed");

    qputenv("LOMIRI_UI_TOOLKIT_THEMES_PATH", "");
    qputenv("XDG_DATA_DIRS", "");
    qputenv("QML2_IMPORT_PATH", "/no/plugins/here:.");

    QScopedPointer<ThemeTestCase> view(new ThemeTestCase("Parent.qml"));
    QVERIFY(view);
    view->setTheme("TestModule.TestTheme");
}

void tst_UCDeprecatedTheme::testPaletteUsed_bug1549830()
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    ThemeTestCase::ignoreWarning("ErroneousPaletteUse.qml", 29, 13, "Unable to assign [undefined] to QColor", 1);
#else
    ThemeTestCase::ignoreWarning("ErroneousPaletteUse.qml", 29, 13, "Unable to assign [undefined] to QColor", 3);
#endif
    QScopedPointer<ThemeTestCase> view(new ThemeTestCase("ErroneousPaletteUse.qml"));
}


QTEST_MAIN(tst_UCDeprecatedTheme)

#include "tst_deprecated_theme_engine.moc"
