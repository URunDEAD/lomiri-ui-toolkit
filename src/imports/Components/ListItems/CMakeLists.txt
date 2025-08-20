# Set the plugin directory for ListItems
set(QML_PLUGIN_DIR "${QML_PLUGIN_BASE_DIR}/Components/ListItems")

# Define QML files for version 1.2
set(QML_FILES_1_2
    "1.2/Base.qml"
    "1.2/Caption.qml"
    "1.2/Divider.qml"
    "1.2/Empty.qml"
    "1.2/Expandable.qml"
    "1.2/ExpandablesColumn.qml"
    "1.2/ListItemHeader.qml"
    "1.2/IconVisual.qml"
    "1.2/ImageWithFallback.qml"
    "1.2/ItemSelector.qml"
    "1.2/LabelVisual.qml"
    "1.2/MultiValue.qml"
    "1.2/ProgressionVisual.qml"
    "1.2/SingleControl.qml"
    "1.2/SingleValue.qml"
    "1.2/Standard.qml"
    "1.2/Subtitled.qml"
    "1.2/ThinDivider.qml"
    "1.2/ValueSelector.qml"
)

# Define QML files for version 1.3
set(QML_FILES_1_3
    "1.3/Base.qml"
    "1.3/Caption.qml"
    "1.3/Divider.qml"
    "1.3/Empty.qml"
    "1.3/Expandable.qml"
    "1.3/ExpandablesColumn.qml"
    "1.3/ListItemHeader.qml"
    "1.3/IconVisual.qml"
    "1.3/ImageWithFallback.qml"
    "1.3/ItemSelector.qml"
    "1.3/LabelVisual.qml"
    "1.3/MultiValue.qml"
    "1.3/ProgressionVisual.qml"
    "1.3/SingleControl.qml"
    "1.3/SingleValue.qml"
    "1.3/Standard.qml"
    "1.3/Subtitled.qml"
    "1.3/ThinDivider.qml"
    "1.3/ValueSelector.qml"
)

# Define other files (if any)
set(OTHER_FILES
    "qmldir"
)

# Combine all QML files
set(ALL_QML_FILES
    ${QML_FILES_1_2}
    ${QML_FILES_1_3}
    ${OTHER_FILES}
)

install(FILES ${QML_FILES_1_2}
    DESTINATION ${QML_PLUGIN_DIR}/1.2
)

install(FILES ${QML_FILES_1_3}
    DESTINATION ${QML_PLUGIN_DIR}/1.3
)
