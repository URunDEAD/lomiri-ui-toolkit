TESTS += $$system(ls tst_*.qml)
!equals(QT_MAJOR_VERSION, 5) {
    TESTS -= tst_alarms.11.qm
    TESTS -= tst_stress_alarmmodel.12.qml
}

include(../qmltest-include-x11.pri)

CONFIG += c++11

SOURCES += tst_visual.cpp tabsmodel.cpp
HEADERS += tabsmodel.h

OTHER_FILES += $$system(ls *.qml)
OTHER_FILES += $$system(ls AppTheme/*)

check.commands += $$PWD/naming.sh
