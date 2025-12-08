set(QML_FILES_0_1
    1.2/Palette.qml
    1.2/PaletteValues.qml
)
set_source_files_properties(${QML_FILES_0_1}
    PROPERTIES
        QT_QML_SOURCE_VERSIONS "0.1"
)

set(QML_FILES_1_3
    1.3/Palette.qml
    1.3/PaletteValues.qml
)
set_source_files_properties(${QML_FILES_1_3}
    PROPERTIES
        QT_QML_SOURCE_VERSIONS "1.3"
)

set(QML_FILES_2_0
    2.0/Palette.qml
    2.0/PaletteValues.qml
)
set_source_files_properties(${QML_FILES_2_0}
    PROPERTIES
        QT_QML_SOURCE_VERSIONS "2.0"
)

qt_add_qml_module(LomiriComponentsThemes
    URI Lomiri.Components.Themes
    VERSION 2.0
    PLUGIN_TARGET LomiriComponentsThemes
    OUTPUT_DIRECTORY "${QML_BUILD_DIR}/Lomiri/Components/Themes"
    NO_GENERATE_EXTRA_QMLDIRS
    NO_PLUGIN_OPTIONAL
    QML_FILES
        ${QML_FILES_0_1}
        ${QML_FILES_1_3}
        ${QML_FILES_2_0}
)

install_qml_module(LomiriComponentsThemes)

add_subdirectory(Ambiance)
add_subdirectory(SuruDark)
add_subdirectory(SuruGradient)
