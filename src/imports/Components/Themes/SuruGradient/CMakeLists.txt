qt_add_qml_module(LomiriComponentsThemesSuruGradient
    URI Lomiri.Components.Themes.SuruGradient
    VERSION 2.0
    PLUGIN_TARGET LomiriComponentsThemesSuruGradient
    OUTPUT_DIRECTORY "${QML_BUILD_DIR}/Lomiri/Components/Themes/SuruGradient"
    NO_GENERATE_EXTRA_QMLDIRS
    NO_PLUGIN_OPTIONAL
    QML_FILES
        2.0/MainViewStyle.qml
        2.0/OptionSelectorStyle.qml
        2.0/Palette.qml
        2.0/TabBarStyle.qml
    RESOURCES
        artwork/chevron@27.png
        artwork/chevron_down@30.png
        artwork/tick@30.png
        parent_theme
        deprecated
)

install_qml_module(LomiriComponentsThemesSuruGradient)
