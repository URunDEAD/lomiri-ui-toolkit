#!/bin/bash
#
# Copyright 2015 Canonical Ltd.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; version 3.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# Based on pbuilderjenkins license checking script.

command -v licensecheck >/dev/null 2>&1 || exit 1

include_files="\.(c(c|pp|xx)?|h(h|pp|xx)?|p(l|m)|php|py(|x)|java|js|vala|qml)$"
exclude_dirs="(3rd_party|qrc_|moc_|_build|include|^build)"
allowed_licenses="(Canonical|UBports|Android|Google|Digia|Qt Company Ltd)"
tab_char="$(echo -e \\t)"
issues=`licensecheck -r * --copyright -m -c "$include_files" -i "$exclude_dirs" | egrep -v "$tab_char.*$allowed_licenses"`

if [ -z "$issues" ]; then
    echo No license problems found.
    exit 0
else
    echo "Found $(echo "$issues" | wc -l) license problem(s):"
    echo "$issues"
    exit 1
fi
