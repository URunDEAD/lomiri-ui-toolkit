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
 * Author: Loïc Molinari <loic.molinari@canonical.com>
 */

#ifndef UCPROPORTIONALSHAPE_P_H
#define UCPROPORTIONALSHAPE_P_H

#include <QtGlobal>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <LomiriToolkit-Qt6/private/uclomirishape_p.h>
#else
#include <LomiriToolkit/private/uclomirishape_p.h>
#endif

UT_NAMESPACE_BEGIN

class LOMIRITOOLKIT_EXPORT UCProportionalShape : public UCLomiriShape
{
    Q_OBJECT

public:
    UCProportionalShape(QQuickItem* parent=0);

private Q_SLOTS:
    void _q_updateWidth();
    void _q_updateHeight();

private:
    void componentComplete() override;

    Q_DISABLE_COPY(UCProportionalShape)
};

UT_NAMESPACE_END

QML_DECLARE_TYPE(UT_PREPEND_NAMESPACE(UCProportionalShape))

#endif  // UCPROPORTIONALSHAPE_P_H
