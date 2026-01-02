set(QML_PLUGIN_DIR "${QML_PLUGIN_BASE_DIR}/Components/Themes")

set(QML_FILES_2_0
     "2.0/Palette.qml"
     "2.0/PaletteValues.qml"
)

set(OTHER_FILES
    "qmldir"
)

# Install files
install(FILES ${QML_FILES_2_0}
    DESTINATION ${QML_PLUGIN_DIR}/2.0
)

install(FILES ${OTHER_FILES}
    DESTINATION ${QML_PLUGIN_DIR}
)

add_subdirectory(Ambiance)
add_subdirectory(SuruDark)
add_subdirectory(SuruGradient)
