#!/bin/sh -euf
#
# Copyright (C) 2023 UBports Foundation
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
#
# Authored by: Ratchanan Srirattanamet <ratchanan@ubports.com>

old_path="${HOME}/.config/ubuntu-ui-toolkit/theme.ini"
new_path="${HOME}/.config/lomiri-ui-toolkit/theme.ini"

if [ -e "$new_path" ]; then
    echo "${new_path} exists. Users might have already configured new settings." \
         "Don't migrate." >&2
    exit 0
fi

if ! [ -e "$old_path" ]; then
    echo "${old_path} doesn't exist. Nothing to do." >&2
    exit 0
fi

mkdir -p "$(dirname "$new_path")"
# Special care has to be taken due to component name change.
sed -e 's/Ubuntu\.Components/Lomiri.Components/' "$old_path" >"$new_path"

echo "Lomiri UI toolkit config migrated." >&2
