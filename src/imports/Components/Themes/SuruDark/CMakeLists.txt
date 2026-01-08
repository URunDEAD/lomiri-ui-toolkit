qt_add_qml_module(LomiriComponentsThemesSuruDark
    URI Lomiri.Components.Themes.SuruDark
    VERSION 2.0
    PLUGIN_TARGET LomiriComponentsThemesSuruDark
    OUTPUT_DIRECTORY "${QML_BUILD_DIR}/Lomiri/Components/Themes/SuruDark"
    NO_GENERATE_EXTRA_QMLDIRS
    NO_PLUGIN_OPTIONAL
    QML_FILES
        2.0/MainViewStyle.qml
        2.0/OptionSelectorStyle.qml
        2.0/Palette.qml
        2.0/SuruDarkNormal.qml
        2.0/SuruDarkSelected.qml
        2.0/TabBarStyle.qml
    RESOURCES
        artwork/chevron@27.png
        parent_theme
)

install_qml_module(LomiriComponentsThemesSuruDark)
