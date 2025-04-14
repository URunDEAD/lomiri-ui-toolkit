/*
 * Copyright (C) 2014 Canonical Ltd.
 *
 * Authors:
 *   Christian Dywan <christian.dywan@canonical.com>
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

#include "filterbehavior_p.h"

UT_NAMESPACE_BEGIN

FilterBehavior::FilterBehavior(QObject *parent)
    : QObject(parent)
    , m_property(QString())
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    , m_pattern(QRegularExpression())
#else
    , m_pattern(QRegExp())
#endif
{

}

QString
FilterBehavior::property() const
{
    return m_property;
}

void
FilterBehavior::setProperty(const QString& property)
{
    m_property = property;
    Q_EMIT propertyChanged();
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
QRegularExpression
#else
QRegExp
#endif
FilterBehavior::pattern() const
{
    return m_pattern;
}

void
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
FilterBehavior::setPattern(QRegularExpression pattern)
#else
FilterBehavior::setPattern(QRegExp pattern)
#endif
{
    m_pattern = pattern;
    Q_EMIT patternChanged();
}

UT_NAMESPACE_END
