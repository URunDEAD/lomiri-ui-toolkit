TEMPLATE = app
QT += \
    core-private \
    gui-private \
    quick \
    quick-private \
    qml \
    testlib \
    LomiriToolkit \
    LomiriToolkit_private \
    LomiriMetrics
CONFIG += no_keywords c++17
SOURCES += launcher.cpp
installPath = $$[QT_INSTALL_PREFIX]/bin
launcher.files = $$OUT_PWD/$$TARGET
launcher.path = $$installPath
launcher.CONFIG = no_check_exist
launcher.files = $$OUT_PWD/$$TARGET
INSTALLS += launcher
