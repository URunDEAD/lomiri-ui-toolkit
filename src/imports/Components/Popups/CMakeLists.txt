set(QML_PLUGIN_DIR "${QML_PLUGIN_BASE_DIR}/Components/Popups")

set(QML_FILES
    "2.0/ActionSelectionPopover.qml"
    "2.0/ComposerSheet.qml"
    "2.0/DefaultSheet.qml"
    "2.0/Dialog.qml"
    "2.0/internalPopupUtils.js"
    "2.0/Popover.qml"
    "2.0/PopupBase.qml"
    "2.0/popupUtils.js"
    "2.0/SheetBase.qml"
)

install(FILES ${QML_FILES} DESTINATION ${QML_PLUGIN_DIR}/2.0/)
install(FILES "qmldir-qt6" DESTINATION ${QML_PLUGIN_DIR}/ RENAME "qmldir")

