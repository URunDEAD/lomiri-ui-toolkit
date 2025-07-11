TARGETPATH = Lomiri/Components/Popups

equals(QT_MAJOR_VERSION, 5) {
    QML_FILES += 1.2/ActionSelectionPopover.qml \
                 1.2/ComposerSheet.qml \
                 1.2/DefaultSheet.qml \
                 1.2/Dialog.qml \
                 1.2/internalPopupUtils.js \
                 1.2/Popover.qml \
                 1.2/PopupBase.qml \
                 1.2/popupUtils.js \
                 1.2/SheetBase.qml \
                 1.3/ActionSelectionPopover.qml \
                 1.3/ComposerSheet.qml \
                 1.3/DefaultSheet.qml \
                 1.3/Dialog.qml \
                 1.3/internalPopupUtils.js \
                 1.3/Popover.qml \
                 1.3/PopupBase.qml \
                 1.3/popupUtils.js \
                 1.3/SheetBase.qml
    DYNAMIC_QMLDIR = $$cat(qmldir-qt5, lines)
} else {
    QML_FILES += 2.0/ActionSelectionPopover.qml \
                 2.0/ComposerSheet.qml \
                 2.0/DefaultSheet.qml \
                 2.0/Dialog.qml \
                 2.0/internalPopupUtils.js \
                 2.0/Popover.qml \
                 2.0/PopupBase.qml \
                 2.0/popupUtils.js \
                 2.0/SheetBase.qml

    DYNAMIC_QMLDIR = $$cat(qmldir-qt6, lines)
}

OTHER_FILES+= qmldir-qt5 \
             qmldir-qt6


load(lomiri_qml_module)
