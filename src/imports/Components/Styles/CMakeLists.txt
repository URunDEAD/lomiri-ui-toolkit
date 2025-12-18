# The original Qt5 qmldir file declared those as version 1.1
# Not sure why they are located in a 1.2 directory.
set(QML_FILES_1_1
    1.2/ComboButtonStyle.qml
    1.2/PullToRefreshStyle.qml
)
set_source_files_properties(${QML_FILES_1_1}
    PROPERTIES
        QT_QML_SOURCE_VERSIONS "1.1"
)

set(QML_FILES_1_3
    1.3/ActionBarStyle.qml
    1.3/ActionItemProperties.qml
    1.3/PageHeadStyle.qml
    1.3/PageHeaderStyle.qml
    1.3/SectionsStyle.qml
    1.3/ToolbarStyle.qml
)
set_source_files_properties(${QML_FILES_1_3}
    PROPERTIES
        QT_QML_SOURCE_VERSIONS "1.3"
)

qt_add_qml_module(LomiriComponentsStyles
    URI Lomiri.Components.Styles
    VERSION 1.3
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
        ${QML_FILES_1_1}
        ${QML_FILES_1_3}
)

target_link_libraries(LomiriComponentsStyles PRIVATE
    LomiriToolkit
    Qt::Gui
    Qt::Quick
)

install_qml_module(LomiriComponentsStyles)
