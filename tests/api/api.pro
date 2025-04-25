TEMPLATE = subdirs

SUBDIRS = Extinct

OTHER_FILES += test.sh

check.commands += env UITK_BUILD_ROOT=$$ROOT_BUILD_DIR
check.commands += $$PWD/test.sh || exit 1;
