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
 */

#ifndef UCMAINVIEWBASE_P_P_H
#define UCMAINVIEWBASE_P_P_H

#include <QtGlobal>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <LomiriToolkit-Qt6/private/ucmainviewbase_p.h>
#else
#include <LomiriToolkit/private/ucmainviewbase_p.h>
#endif

#include <QtQml/QQmlProperty>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <LomiriToolkit-Qt6/private/ucpagetreenode_p_p.h>
#else
#include <LomiriToolkit/private/ucpagetreenode_p_p.h>
#endif

UT_NAMESPACE_BEGIN

class UCMainViewBase;
class UCActionManager;
class UCPopupContext;

class UCMainViewBasePrivate : public UCPageTreeNodePrivate
{
    Q_DECLARE_PUBLIC(UCMainViewBase)

public:
    UCMainViewBasePrivate();
    void init();

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    enum PropertyFlags {
        CustomHeaderColor     = 0x01,
        CustomBackgroundColor = 0x02,
        CustomFooterColor     = 0x04
    };

    void _q_headerColorBinding (const QColor &col);
    void _q_footerColorBinding (const QColor &col);
#endif
    void doAutoTheme();
    void _q_updateWindow();

    QString m_applicationName;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QColor m_headerColor;
    QColor m_footerColor;
#endif
    QColor m_backgroundColor;
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    UCActionManager *m_actionManager = nullptr;
#endif
    UCPopupContext* m_actionContext = nullptr;
    qint8 m_flags;
    bool m_anchorToKeyboard:1;

};

UT_NAMESPACE_END

#endif // UCMAINVIEWBASE_P_P_H
