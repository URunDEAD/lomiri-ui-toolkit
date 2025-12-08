# Normally, LomiriLayouts should be a module target that is loaded at runtime,
# however we have some tests that need to link to it, so as a workaround we
# make it a shared library target here.
add_library(LomiriLayouts SHARED)

qt_add_qml_module(LomiriLayouts
    URI Lomiri.Layouts
    VERSION 1.0
    PLUGIN_TARGET LomiriLayouts
    CLASS_NAME PluginPlugin
    OUTPUT_DIRECTORY "${QML_BUILD_DIR}/Lomiri/Layouts"
    NO_GENERATE_EXTRA_QMLDIRS
    NO_GENERATE_QMLTYPES
    NO_GENERATE_PLUGIN_SOURCE
    NO_PLUGIN_OPTIONAL
    SOURCES
        plugin/propertychanges.cpp
        plugin/propertychanges_p.h
        plugin/ulconditionallayout.cpp
        plugin/ulconditionallayout.h
        plugin/ulconditionallayout_p.h
        plugin/ulitemlayout.cpp
        plugin/ulitemlayout.h
        plugin/ullayouts.cpp
        plugin/ullayouts.h
        plugin/ullayouts_p.h
        plugin/ullayoutsattached.cpp
        plugin/ulplugin.cpp
        plugin/ulplugin.h
)

target_include_directories(LomiriLayouts PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/plugin"
)

target_link_libraries(LomiriLayouts PRIVATE
    Qt::Core
    Qt::CorePrivate
    Qt::Gui
    Qt::GuiPrivate
    Qt::QmlPrivate
    Qt::QuickPrivate
)

install_qml_module(LomiriLayouts)
