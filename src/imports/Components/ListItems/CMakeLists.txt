# File name doesn't match QML type name
set_source_files_properties(2.0/ListItemHeader.qml
    PROPERTIES
        QT_QML_SOURCE_TYPENAME Header
)

qt_add_qml_module(LomiriComponentsListItems
    URI Lomiri.Components.ListItems
    VERSION 2.0
    PLUGIN_TARGET LomiriComponentsListItems
    OUTPUT_DIRECTORY "${QML_BUILD_DIR}/Lomiri/Components/ListItems"
    NO_GENERATE_EXTRA_QMLDIRS
    NO_PLUGIN_OPTIONAL
    QML_FILES
        2.0/Base.qml
        2.0/Caption.qml
        2.0/Divider.qml
        2.0/Empty.qml
        2.0/Expandable.qml
        2.0/ExpandablesColumn.qml
        2.0/IconVisual.qml
        2.0/ImageWithFallback.qml
        2.0/ItemSelector.qml
        2.0/LabelVisual.qml
        2.0/ListItemHeader.qml
        2.0/MultiValue.qml
        2.0/ProgressionVisual.qml
        2.0/SingleControl.qml
        2.0/SingleValue.qml
        2.0/Standard.qml
        2.0/Subtitled.qml
        2.0/ThinDivider.qml
        2.0/ValueSelector.qml
)

install_qml_module(LomiriComponentsListItems)
