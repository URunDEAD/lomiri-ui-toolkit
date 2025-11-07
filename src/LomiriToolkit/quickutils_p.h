/*
 * Copyright 2012 Canonical Ltd.
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

#ifndef QUICKUTILS_P_H
#define QUICKUTILS_P_H

#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtQuick/QQuickView>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <LomiriToolkit-Qt6/lomiritoolkitglobal.h>
#else
#include <LomiriToolkit/lomiritoolkitglobal.h>
#endif

class QQuickItem;
class QQmlEngine;
class QQmlComponent;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
class LomiriInputInfoManager;
class LomiriInputDevice;
#else
class QInputInfoManager;
class QInputDevice;
#endif

UT_NAMESPACE_BEGIN

class LOMIRITOOLKIT_EXPORT QuickUtils : public QObject
{
    Q_OBJECT
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    Q_PROPERTY(QQuickItem *rootObject READ rootObject NOTIFY rootObjectChanged)
#endif
    Q_PROPERTY(QString inputMethodProvider READ inputMethodProvider)
    Q_PROPERTY(bool touchScreenAvailable READ touchScreenAvailable NOTIFY touchScreenAvailableChanged)
    Q_PROPERTY(bool mouseAttached MEMBER m_mouseAttached WRITE setMouseAttached NOTIFY mouseAttachedChanged)
    Q_PROPERTY(bool keyboardAttached MEMBER m_keyboardAttached WRITE setKeyboardAttached NOTIFY keyboardAttachedChanged)
public:
    static QuickUtils *instance(QObject *parent = Q_NULLPTR)
    {
        if (!m_instance) {
            if (!parent) {
                qFatal("Creating QuickUtils singleton requires a parent object!");
            }
            m_instance = new QuickUtils(parent);
        }
        return m_instance;
    }
    ~QuickUtils() { m_instance = Q_NULLPTR; }

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QQuickItem *rootObject();
#endif
    Q_INVOKABLE QQuickItem *rootItem(QObject *object);
    QString inputMethodProvider() const;
    bool touchScreenAvailable() const;

    Q_INVOKABLE static QString className(QObject *item);
    Q_REVISION(1) Q_INVOKABLE static bool inherits(QObject *object, const QString &fromClass);
    QObject* createQmlObject(const QUrl &url, QQmlEngine *engine);
    static bool showDeprecationWarnings();
    static bool descendantItemOf(QQuickItem *item, const QQuickItem *parent);
    Q_INVOKABLE static QQuickItem *firstFocusableChild(QQuickItem *item);
    Q_INVOKABLE static QQuickItem *lastFocusableChild(QQuickItem *item);

    bool mouseAttached()
    {
        return m_mouseAttached;
    }
    bool keyboardAttached()
    {
        return m_keyboardAttached;
    }

Q_SIGNALS:
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    void rootObjectChanged();
#endif
    void activated();
    void deactivated();
    void touchScreenAvailableChanged();
    void mouseAttachedChanged();
    void keyboardAttachedChanged();

protected:
    bool eventFilter(QObject *, QEvent *) override;

private:
    explicit QuickUtils(QObject *parent = 0);
    QPointer<QQuickWindow> m_rootWindow;
    QPointer<QQuickView> m_rootView;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    LomiriInputInfoManager *m_inputInfo;
#else
    QInputInfoManager *m_inputInfo;
#endif
    QStringList m_omitIM;
    QSet<QString> m_mice;
    QSet<QString> m_keyboards;
    bool m_mouseAttached:1;
    bool m_keyboardAttached:1;
    bool m_explicitMouseAttached:1;
    bool m_explicitKeyboardAttached:1;

    static QuickUtils *m_instance;

    void lookupQuickView();
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void registerDevice(LomiriInputDevice *device, const QString &deviceId);
#else
    void registerDevice(QInputDevice *device, const QString &deviceId);
#endif
    void setMouseAttached(bool set);
    void setKeyboardAttached(bool set);
private Q_SLOTS:
    void onInputInfoReady();
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    void onDeviceAdded(LomiriInputDevice *device);
#else
    void onDeviceAdded(QInputDevice *device);
#endif
    void onDeviceRemoved(const QString deviceId);
};

#define UC_QML_DEPRECATION_WARNING(msg) \
    { \
        static bool loggedOnce = false; \
        if (!loggedOnce) { \
            if (QuickUtils::showDeprecationWarnings()) { \
                qmlWarning(this) << msg; \
            } \
        } \
    }

UT_NAMESPACE_END

#endif // QUICKUTILS_P_H
