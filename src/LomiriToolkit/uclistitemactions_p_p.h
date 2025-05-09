/*
 * Copyright 2014 Canonical Ltd.
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

#ifndef UCLISTITEMACTIONS_P_P_H
#define UCLISTITEMACTIONS_P_P_H

#include <QtGlobal>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <LomiriToolkitQt6/private/uclistitemactions_p.h>
#else
#include <LomiriToolkit/private/uclistitemactions_p.h>
#endif

#include <QtCore/private/qobject_p.h>
#include <QtQml/QQmlListProperty>

UT_NAMESPACE_BEGIN

class UCListItem;
class UCListItemActionsPrivate : public QObjectPrivate {
    Q_DECLARE_PUBLIC(UCListItemActions)
public:
    UCListItemActionsPrivate();
    ~UCListItemActionsPrivate();
    static UCListItemActionsPrivate* get(UCListItemActions *actions)
    {
        return actions ? actions->d_func() : 0;
    }

    QQmlComponent *delegate;
    QList<UCAction*> actions;
    QList<QObject*> data;

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    static qsizetype actions_count(QQmlListProperty<UCAction> *p);
    static UCAction *actions_at(QQmlListProperty<UCAction>*, qsizetype);
#else
    static int actions_count(QQmlListProperty<UCAction> *p);
    static UCAction *actions_at(QQmlListProperty<UCAction>*, int);
#endif
    static void actions_append(QQmlListProperty<UCAction> *p, UCAction *v);
    static void actions_clear(QQmlListProperty<UCAction>*);
};

UT_NAMESPACE_END

#endif // UCLISTITEMACTIONS_P_P_H
