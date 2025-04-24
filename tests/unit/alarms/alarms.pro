include(../test-include.pri)
QT += organizer
!equals(QT_MAJOR_VERSION, 5) {
    QT += core5compat
}
SOURCES += tst_alarms.cpp
