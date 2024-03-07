TEMPLATE = subdirs
SUBDIRS += unit autopilot
SUBDIRS += api

OTHER_FILES += qmlapicheck.sh

check.commands += $$PWD/qmlapicheck.sh || exit 1;
