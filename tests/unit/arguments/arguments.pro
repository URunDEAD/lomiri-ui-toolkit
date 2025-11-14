include(../test-include.pri)
!equals(QT_MAJOR_VERSION, 5) {
    QT += core5compat
}
SOURCES += tst_arguments.cpp
