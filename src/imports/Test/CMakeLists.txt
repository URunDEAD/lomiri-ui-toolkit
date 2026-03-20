set_source_files_properties(LomiriTestCase20.qml
    PROPERTIES
        QT_QML_SOURCE_TYPENAME LomiriTestCase
)

# Normally, LomiriTest should be a module target that is loaded at runtime,
# however we have some tests that need to link to it, so as a workaround we
# make it a shared library target here.
add_library(LomiriTest SHARED)

qt_add_qml_module(LomiriTest
    URI Lomiri.Test
    VERSION 2.0
    PLUGIN_TARGET LomiriTest
    CLASS_NAME TestPlugin
    OUTPUT_DIRECTORY "${QML_BUILD_DIR}/Lomiri/Test"
    NO_GENERATE_EXTRA_QMLDIRS
    NO_GENERATE_QMLTYPES
    NO_GENERATE_PLUGIN_SOURCE
    NO_PLUGIN_OPTIONAL
    SOURCES
        plugin/testplugin.cpp
        plugin/testplugin.h
        plugin/uctestcase.cpp
        plugin/uctestcase.h
        plugin/uctestextras.cpp
        plugin/uctestextras.h
    QML_FILES
        LomiriTestCase20.qml
)

target_compile_definitions(LomiriTest PRIVATE
    LOMIRI_QML_IMPORT_PATH="/qml"
)

target_include_directories(LomiriTest PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/plugin"
)

target_link_libraries(LomiriTest PRIVATE
    LomiriGestures
    LomiriToolkit
    Qt::Core
    Qt::CorePrivate
    Qt::Gui
    Qt::GuiPrivate
    Qt::Qml
    Qt::QmlPrivate
    Qt::QuickPrivate
    Qt::Test
)

install_qml_module(LomiriTest)
