#!/bin/bash
#
# Copyright 2012 Canonical Ltd.
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

# FIXME(loicm)
# Since the new src/ layout (src/imports/ for QML modules and src/ for libs),
# the parsing order of the source files has changed. qdoc first stores all the
# file names and then does a sort (QMap iteration) which makes
# ucmainviewbase.cpp parsed before MainView.qml (LomiriToolkit is sorted before
# imports ASCII-wise). The result is that qdoc is unable to link to properties
# inherited from UCMainViewBase like applicationName, so constructs like that
# don't work: \l MainView::applicationName...

QDOC="$1"
QHELPGENERATOR="$2"
DOC_PATH="$3"
if [ -z "$SRC" ] || [ -z "$DOC_PATH" ]; then
    echo "Error - SRC and DOC_PATH must be defined"
    exit 1
fi

mkdir -p "$DOC_PATH"

# Offline docs for QtCreator
"$QDOC" "$SRC/lomiri-ui-toolkit-qtcreator.qdocconf" 2> "$DOC_PATH/qdoc.log"
# FIXME: With Qt 5.2 this warning shows up, forcibly omit it from errors
grep -v "error: Output file already exists; overwriting" "$DOC_PATH/qdoc.log" | grep "error: " > "$DOC_PATH/qdoc.err"
cat "$DOC_PATH/qdoc.err"
test ! -s $DOC_PATH/qdoc.err || exit 1
echo docs: Offline done.

"$QHELPGENERATOR" -o "$DOC_PATH/lomiriuserinterfacetoolkit.qch" "$DOC_PATH/html/lomiriuserinterfacetoolkit.qhp" | grep -v ' does not exist! Skipping it'
echo "docs: qch done: $DOC_PATH"

# Second qdoc pass, this time with indexes for cross-referencing.
# If we don't do this, bugs in docs from other modules (Qt, Unity) fail our build
"$QDOC" "$SRC/lomiri-ui-toolkit-indexes.qdocconf" 2> "$DOC_PATH/qdoc.log"
grep "error: " "$DOC_PATH/qdoc.log" | grep -v "error: Output file already exists" > "$DOC_PATH/qdoc.err"
cat "$DOC_PATH/qdoc.err"
test ! -s "$DOC_PATH/qdoc.err" || exit 1
echo docs: Cross-referenced done.
