TEMPLATE = subdirs
SUBDIRS += unit autopilot
SUBDIRS += api

OTHER_FILES += qmlapicheck.sh

check.commands += env UITK_BUILD_ROOT=$$ROOT_BUILD_DIR $$PWD/qmlapicheck.sh || exit 1;
