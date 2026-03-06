set_source_files_properties(2.0/PopupUtils.js
    PROPERTIES
        QT_QML_SKIP_QMLDIR_ENTRY FALSE
)

qt_add_qml_module(LomiriComponentsPopups
    URI Lomiri.Components.Popups
    VERSION 2.0
    PLUGIN_TARGET LomiriComponentsPopups
    OUTPUT_DIRECTORY "${QML_BUILD_DIR}/Lomiri/Components/Popups"
    NO_GENERATE_EXTRA_QMLDIRS
    NO_PLUGIN_OPTIONAL
    QML_FILES
        2.0/ActionSelectionPopover.qml
        2.0/ComposerSheet.qml
        2.0/DefaultSheet.qml
        2.0/Dialog.qml
        2.0/internalPopupUtils.js
        2.0/Popover.qml
        2.0/PopupBase.qml
        2.0/PopupUtils.js
        2.0/SheetBase.qml
)

install_qml_module(LomiriComponentsPopups)

# Woraround for https://qt-project.atlassian.net/browse/QTBUG-144829
# The qmldir "prefer" directive is ignored for scripts when using a
# qualified import, so for now we install the JavaScript file.
install(FILES "2.0/PopupUtils.js"
    DESTINATION "${QML_INSTALL_DIR}/Lomiri/Components/Popups/2.0"
)
