CXX_MODULE = qml
TARGET  = LomiriTest
TARGETPATH = Lomiri/Test
IMPORT_VERSION = 0.1
QT += qml LomiriGestures-private LomiriToolkit

#QMAKE_DOCS = $$PWD/doc/qtquicklayouts.qdocconf

include(plugin/plugin.pri)

QML_FILES += \
    LomiriTestCase.qml \
    LomiriTestCase13.qml \

DYNAMIC_QMLDIR = $$cat(qmldir-qt5, lines)
OTHER_FILES+= qmldir-qt5

load(lomiri_qml_plugin)

