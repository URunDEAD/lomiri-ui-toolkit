qt_add_qml_module(LomiriComponentsThemes
    URI Lomiri.Components.Themes
    VERSION 2.0
    PLUGIN_TARGET LomiriComponentsThemes
    OUTPUT_DIRECTORY "${QML_BUILD_DIR}/Lomiri/Components/Themes"
    NO_GENERATE_EXTRA_QMLDIRS
    NO_PLUGIN_OPTIONAL
    QML_FILES
        2.0/Palette.qml
        2.0/PaletteValues.qml
)

install_qml_module(LomiriComponentsThemes)

add_subdirectory(Ambiance)
add_subdirectory(SuruDark)
add_subdirectory(SuruGradient)
