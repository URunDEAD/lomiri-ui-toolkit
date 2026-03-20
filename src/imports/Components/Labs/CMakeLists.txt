qt_add_qml_module(LomiriComponentsLabs
    URI Lomiri.Components.Labs
    VERSION 1.0
    PLUGIN_TARGET LomiriComponentsLabs
    CLASS_NAME LomiriComponentsLabsPlugin
    OUTPUT_DIRECTORY "${QML_BUILD_DIR}/Lomiri/Components/Labs"
    NO_GENERATE_EXTRA_QMLDIRS
    NO_GENERATE_QMLTYPES
    NO_GENERATE_PLUGIN_SOURCE
    NO_PLUGIN_OPTIONAL
    SOURCES
        plugin/plugin.cpp
)

target_include_directories(LomiriComponentsLabs PRIVATE
    plugin
)

target_link_libraries(LomiriComponentsLabs PRIVATE
    LomiriToolkit
    Qt::Core
    Qt::Gui
    Qt::Qml
    Qt::Quick
)

install_qml_module(LomiriComponentsLabs)
