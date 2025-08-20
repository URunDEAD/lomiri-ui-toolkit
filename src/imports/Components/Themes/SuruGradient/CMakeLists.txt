set(QML_PLUGIN_DIR "${QML_PLUGIN_BASE_DIR}/Components/Themes/SuruGradient")

set(QML_FILES
    "MainViewStyle.qml"
    "OptionSelectorStyle.qml"
    "Palette.qml"
    "TabBarStyle.qml"
)


set(OTHER_FILES
    "parent_theme"
    "deprecated"
    "artwork/chevron@27.png"
    "artwork/chevron_down@30.png"
    "artwork/tick@30.png"
    "qmldir"
)

set(ALL_QML_FILES
    ${QML_FILES}
    ${OTHER_FILES}
)

install(FILES ${ALL_QML_FILES}
    DESTINATION ${QML_PLUGIN_DIR}
)
