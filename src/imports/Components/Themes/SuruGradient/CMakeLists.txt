set(QML_FILES_1_1
    MainViewStyle.qml
    OptionSelectorStyle.qml
    Palette.qml
    TabBarStyle.qml
)
set_source_files_properties(${QML_FILES_1_1}
    PROPERTIES
        QT_QML_SOURCE_VERSIONS "1.1"
)

qt_add_qml_module(LomiriComponentsThemesSuruGradient
    URI Lomiri.Components.Themes.SuruGradient
    VERSION 1.1
    PLUGIN_TARGET LomiriComponentsThemesSuruGradient
    OUTPUT_DIRECTORY "${QML_BUILD_DIR}/Lomiri/Components/Themes/SuruGradient"
    NO_GENERATE_EXTRA_QMLDIRS
    NO_PLUGIN_OPTIONAL
    QML_FILES
        ${QML_FILES_1_1}
    RESOURCES
        artwork/chevron@27.png
        artwork/chevron_down@30.png
        artwork/tick@30.png
        parent_theme
        deprecated
)

install_qml_module(LomiriComponentsThemesSuruGradient)
