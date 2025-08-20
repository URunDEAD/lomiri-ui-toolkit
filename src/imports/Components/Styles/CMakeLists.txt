add_library(LomiriComponentsStyles SHARED
    plugin/plugin.cpp
)

target_link_libraries(LomiriComponentsStyles PRIVATE
    LomiriToolkit
    Qt::Gui
    Qt::Quick
)

set(QML_PLUGIN_DIR "${QML_PLUGIN_BASE_DIR}/Components/Styles")

set(QML_FILES_1_2
    "1.2/ComboButtonStyle.qml"
    "1.2/PageHeadStyle.qml"
    "1.2/PullToRefreshStyle.qml"
)

set(QML_FILES_1_3
    "1.3/PageHeadStyle.qml"
    "1.3/ActionBarStyle.qml"
    "1.3/PageHeaderStyle.qml"
    "1.3/ToolbarStyle.qml"
    "1.3/SectionsStyle.qml"
    "1.3/ActionItemProperties.qml"
)

set(OTHER_FILES
    "qmldir"
)

# Install files
install(TARGETS LomiriComponentsStyles
    DESTINATION ${QML_PLUGIN_DIR}
)

install(FILES ${QML_FILES_1_2}
    DESTINATION ${QML_PLUGIN_DIR}/1.2
)

install(FILES ${QML_FILES_1_3}
    DESTINATION ${QML_PLUGIN_DIR}/1.3
)

install(FILES ${OTHER_FILES}
    DESTINATION ${QML_PLUGIN_DIR}
)
