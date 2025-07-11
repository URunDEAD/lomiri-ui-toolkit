TARGETPATH = Lomiri/Components/ListItems

equals(QT_MAJOR_VERSION, 5) {
    QML_FILES += 1.2/Base.qml \
                 1.2/Caption.qml \
                 1.2/Divider.qml \
                 1.2/Empty.qml \
                 1.2/Expandable.qml \
                 1.2/ExpandablesColumn.qml \
                 1.2/ListItemHeader.qml \
                 1.2/IconVisual.qml \
                 1.2/ImageWithFallback.qml \
                 1.2/ItemSelector.qml \
                 1.2/LabelVisual.qml \
                 1.2/MultiValue.qml \
                 1.2/ProgressionVisual.qml \
                 1.2/SingleControl.qml \
                 1.2/SingleValue.qml \
                 1.2/Standard.qml \
                 1.2/Subtitled.qml \
                 1.2/ThinDivider.qml \
                 1.2/ValueSelector.qml \
                 1.3/Base.qml \
                 1.3/Caption.qml \
                 1.3/Divider.qml \
                 1.3/Empty.qml \
                 1.3/Expandable.qml \
                 1.3/ExpandablesColumn.qml \
                 1.3/ListItemHeader.qml \
                 1.3/IconVisual.qml \
                 1.3/ImageWithFallback.qml \
                 1.3/ItemSelector.qml \
                 1.3/LabelVisual.qml \
                 1.3/MultiValue.qml \
                 1.3/ProgressionVisual.qml \
                 1.3/SingleControl.qml \
                 1.3/SingleValue.qml \
                 1.3/Standard.qml \
                 1.3/Subtitled.qml \
                 1.3/ThinDivider.qml \
                 1.3/ValueSelector.qml
    DYNAMIC_QMLDIR = $$cat(qmldir-qt5, lines)
} else {
    QML_FILES += 2.0/Base.qml \
                 2.0/Caption.qml \
                 2.0/Divider.qml \
                 2.0/Empty.qml \
                 2.0/Expandable.qml \
                 2.0/ExpandablesColumn.qml \
                 2.0/ListItemHeader.qml \
                 2.0/IconVisual.qml \
                 2.0/ImageWithFallback.qml \
                 2.0/ItemSelector.qml \
                 2.0/LabelVisual.qml \
                 2.0/MultiValue.qml \
                 2.0/ProgressionVisual.qml \
                 2.0/SingleControl.qml \
                 2.0/SingleValue.qml \
                 2.0/Standard.qml \
                 2.0/Subtitled.qml \
                 2.0/ThinDivider.qml \
                 2.0/ValueSelector.qml
    DYNAMIC_QMLDIR = $$cat(qmldir-qt6, lines)
}

OTHER_FILES+= qmldir-qt5 \
             qmldir-qt6

load(lomiri_qml_module)
