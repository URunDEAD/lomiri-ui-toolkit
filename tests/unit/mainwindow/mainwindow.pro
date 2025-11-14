CONFIG += custom_qpa   # needed by test to set device pixel ratio correctly
include(../test-include.pri)

QT += gui
!equals(QT_MAJOR_VERSION, 5) {
    QT += core5compat
}
SOURCES += tst_mainwindow.cpp
