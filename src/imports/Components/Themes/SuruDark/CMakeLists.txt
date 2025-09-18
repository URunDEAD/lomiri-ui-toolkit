set(QML_PLUGIN_DIR "${QML_PLUGIN_BASE_DIR}/Components/Themes/SuruDark")

set(QML_FILES_1_2
    "1.2/MainViewStyle.qml"
    "1.2/OptionSelectorStyle.qml"
    "1.2/Palette.qml"
    "1.2/TabBarStyle.qml"
)

set(QML_FILES_1_3
    "1.3/OptionSelectorStyle.qml"
    "1.3/SuruDarkNormal.qml"
    "1.3/SuruDarkSelected.qml"
    "1.3/Palette.qml"
    "1.3/TabBarStyle.qml"
)

set(ARTWORK_FILES
    "artwork/chevron@27.png"
)

set(OTHER_FILES
    "parent_theme"
    "qmldir"
)

# Install files
install(FILES ${QML_FILES_1_2}
    DESTINATION ${QML_PLUGIN_DIR}/1.2
)

install(FILES ${QML_FILES_1_3}
    DESTINATION ${QML_PLUGIN_DIR}/1.3
)

install(FILES ${OTHER_FILES}
    DESTINATION ${QML_PLUGIN_DIR}
)

install(FILES ${ARTWORK_FILES}
    DESTINATION ${QML_PLUGIN_DIR}/artwork
)
