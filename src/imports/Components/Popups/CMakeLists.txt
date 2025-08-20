set(QML_PLUGIN_DIR "${QML_PLUGIN_BASE_DIR}/Components/Popups")

set(QML_FILES_1_2
    "1.2/ActionSelectionPopover.qml"
    "1.2/ComposerSheet.qml"
    "1.2/DefaultSheet.qml"
    "1.2/Dialog.qml"
    "1.2/internalPopupUtils.js"
    "1.2/Popover.qml"
    "1.2/PopupBase.qml"
    "1.2/popupUtils.js"
    "1.2/SheetBase.qml"
)

set(QML_FILES_1_3
    "1.3/ActionSelectionPopover.qml"
    "1.3/ComposerSheet.qml"
    "1.3/DefaultSheet.qml"
    "1.3/Dialog.qml"
    "1.3/internalPopupUtils.js"
    "1.3/Popover.qml"
    "1.3/PopupBase.qml"
    "1.3/popupUtils.js"
    "1.3/SheetBase.qml"

    "1.3/internalPopupUtils.js"
    "1.3/popupUtils.js"
)

set(OTHER_FILES
    "qmldir"

)

set(ALL_QML_FILES
    ${QML_FILES_1_2}
    ${QML_FILES_1_3}
    ${OTHER_FILES}
)

install(FILES ${ARTWORK_FILES}
    DESTINATION ${QML_PLUGIN_DIR}/artwork
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
