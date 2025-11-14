# Set the plugin directory for ListItems
set(QML_PLUGIN_DIR "${QML_PLUGIN_BASE_DIR}/Components/ListItems")

# Define QML files for version 1.2
set(QML_FILES
    "2.0/Base.qml"
    "2.0/Caption.qml"
    "2.0/Divider.qml"
    "2.0/Empty.qml"
    "2.0/Expandable.qml"
    "2.0/ExpandablesColumn.qml"
    "2.0/ListItemHeader.qml"
    "2.0/IconVisual.qml"
    "2.0/ImageWithFallback.qml"
    "2.0/ItemSelector.qml"
    "2.0/LabelVisual.qml"
    "2.0/MultiValue.qml"
    "2.0/ProgressionVisual.qml"
    "2.0/SingleControl.qml"
    "2.0/SingleValue.qml"
    "2.0/Standard.qml"
    "2.0/Subtitled.qml"
    "2.0/ThinDivider.qml"
    "2.0/ValueSelector.qml"
)

install(FILES ${QML_FILES} DESTINATION ${QML_PLUGIN_DIR}/2.0)
install(FILES "qmldir-qt6" DESTINATION ${QML_PLUGIN_DIR}/ RENAME "qmldir")
