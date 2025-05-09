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

#ifndef FILTERBEHAVIOR_P_H
#define FILTERBEHAVIOR_P_H

#include <QtCore/QSortFilterProxyModel>
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QRegularExpression>
#else
#include <QRegExp>
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <LomiriToolkitQt6/lomiritoolkitglobal.h>
#else
#include <LomiriToolkit/lomiritoolkitglobal.h>
#endif

UT_NAMESPACE_BEGIN

class LOMIRITOOLKIT_EXPORT FilterBehavior : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString property READ property WRITE setProperty NOTIFY propertyChanged)
    Q_PROPERTY(
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
               QRegularExpression
#else
	       QRegExp
#endif
	       pattern READ pattern WRITE setPattern NOTIFY patternChanged)

public:
    explicit FilterBehavior(QObject *parent = 0);

    QString property() const;
    void setProperty(const QString& property);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QRegularExpression pattern() const;
    void setPattern(QRegularExpression pattern);
#else
    QRegExp pattern() const;
    void setPattern(QRegExp pattern);
#endif

Q_SIGNALS:
    void propertyChanged();
    void patternChanged();

private:
    QString m_property;
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QRegularExpression m_pattern;
#else
    QRegExp m_pattern;
#endif
};

UT_NAMESPACE_END

#endif // FILTERBEHAVIOR_P_H
