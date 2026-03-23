qt_add_qml_module(LomiriComponentsStyles
    URI Lomiri.Components.Styles
    VERSION 2.0
    PLUGIN_TARGET LomiriComponentsStyles
    CLASS_NAME LomiriComponentsStylesPlugin
    OUTPUT_DIRECTORY "${QML_BUILD_DIR}/Lomiri/Components/Styles"
    NO_GENERATE_EXTRA_QMLDIRS
    NO_GENERATE_QMLTYPES
    NO_GENERATE_PLUGIN_SOURCE
    NO_PLUGIN_OPTIONAL
    SOURCES
        plugin/plugin.cpp
    QML_FILES
        2.0/ActionBarStyle.qml
        2.0/ActionItemProperties.qml
        2.0/ComboButtonStyle.qml
        2.0/PageHeaderStyle.qml
        2.0/PageHeadStyle.qml
        2.0/PullToRefreshStyle.qml
        2.0/SectionsStyle.qml
        2.0/ToolbarStyle.qml
)

target_link_libraries(LomiriComponentsStyles PRIVATE
    LomiriToolkit
    Qt::Gui
    Qt::Quick
)

install_qml_module(LomiriComponentsStyles)
