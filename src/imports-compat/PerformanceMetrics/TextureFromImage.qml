/*
 * Copyright 2021 UBports Foundation.
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

import Lomiri.PerformanceMetrics 1.0

// DeprecationPrinter
import QtQml 2.9
import Lomiri.Components.Private 1.3

TextureFromImage {
    Component.onCompleted: {
        DeprecationPrinter.printDeprecation(
            DeprecationPrinter.PERFORMANCEMETRICS);
    }
}
