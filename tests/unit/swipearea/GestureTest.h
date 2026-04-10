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
 */

#ifndef GESTURETEST_H
#define GESTURETEST_H

#include <functional>

#include <QtGui/QTouchEvent>
#include <QtQuick/QQuickItem>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <LomiriGestures-Qt6/lomirigesturesglobal.h>
#else
#include <LomiriGestures/lomirigesturesglobal.h>
#endif

class LomiriTestCase;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
class QTouchDevice;
#else
class QPointingDevice;
#endif

UG_FORWARD_DECLARE_CLASS(FakeTimerFactory)
UG_FORWARD_DECLARE_CLASS(TouchRegistry)

/*
    The common stuff among tests come here
 */

class TouchMemento {
public:
    TouchMemento(const QTouchEvent *touchEvent);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    Qt::TouchPointStates touchPointStates;
    QList<QTouchEvent::TouchPoint> touchPoints;
#else
    QEventPoint::States touchPointStates;
    QList<QEventPoint> touchPoints;
#endif

    bool containsTouchWithId(int touchId) const;
};

class DummyItem : public QQuickItem
{
    Q_OBJECT
public:
    DummyItem(QQuickItem *parent = 0);

    QList<TouchMemento> touchEvents;
    std::function<void(QTouchEvent*)> touchEventHandler;
    std::function<void(QMouseEvent*)> mousePressEventHandler;
    std::function<void(QMouseEvent*)> mouseMoveEventHandler;
    std::function<void(QMouseEvent*)> mouseReleaseEventHandler;
    std::function<void(QMouseEvent*)> mouseDoubleClickEventHandler;
protected:
    void touchEvent(QTouchEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
private:
    static void defaultTouchEventHandler(QTouchEvent *event);
    static void defaultMouseEventHandler(QMouseEvent *event);
};

class GestureTest : public QObject
{
    Q_OBJECT
public:
    // \param qmlFilename name of the qml file to be loaded by the QQuickView
    GestureTest(const QString &qmlFilename);

protected Q_SLOTS:
    void initTestCase(); // will be called before the first test function is executed
    virtual void init(); // called right before each and every test function is executed
    virtual void cleanup(); // called right after each and every test function is executed

protected:
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QTouchDevice *m_device;
#else
    QPointingDevice *m_device;
#endif
    LomiriTestCase *m_view;
    TouchRegistry *m_touchRegistry;
    UG_PREPEND_NAMESPACE(FakeTimerFactory)  *m_fakeTimerFactory;
    QString m_qmlFilename;
};

#endif // GESTURETEST_H
