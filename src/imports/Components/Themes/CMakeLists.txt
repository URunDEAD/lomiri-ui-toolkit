set(QML_PLUGIN_DIR "${QML_PLUGIN_BASE_DIR}/Components/Themes")

set(QML_FILES_1_2
    "1.2/Palette.qml"
    "1.2/PaletteValues.qml"
)

set(QML_FILES_1_3
    "1.3/Palette.qml"
    "1.3/PaletteValues.qml"
)

set(OTHER_FILES
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

add_subdirectory(Ambiance)
add_subdirectory(SuruDark)
add_subdirectory(SuruGradient)
