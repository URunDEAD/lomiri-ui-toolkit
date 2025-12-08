# The original Qt5 qmldir file declared those as version 1.1
# Not sure why they are located in a 1.2 directory.
set(QML_FILES_1_1
    1.2/MainViewStyle.qml
    1.2/OptionSelectorStyle.qml
    1.2/Palette.qml
    1.2/TabBarStyle.qml
)
set_source_files_properties(${QML_FILES_1_1}
    PROPERTIES
        QT_QML_SOURCE_VERSIONS "1.1"
)

set(QML_FILES_1_3
    1.3/OptionSelectorStyle.qml
    1.3/Palette.qml
    1.3/SuruDarkNormal.qml
    1.3/SuruDarkSelected.qml
    1.3/TabBarStyle.qml
)
set_source_files_properties(${QML_FILES_1_3}
    PROPERTIES
        QT_QML_SOURCE_VERSIONS "1.3"
)

qt_add_qml_module(LomiriComponentsThemesSuruDark
    URI Lomiri.Components.Themes.SuruDark
    VERSION 1.3
    PLUGIN_TARGET LomiriComponentsThemesSuruDark
    OUTPUT_DIRECTORY "${QML_BUILD_DIR}/Lomiri/Components/Themes/SuruDark"
    NO_GENERATE_EXTRA_QMLDIRS
    NO_PLUGIN_OPTIONAL
    QML_FILES
        ${QML_FILES_1_1}
        ${QML_FILES_1_3}
    RESOURCES
        artwork/chevron@27.png
        parent_theme
)

install_qml_module(LomiriComponentsThemesSuruDark)
