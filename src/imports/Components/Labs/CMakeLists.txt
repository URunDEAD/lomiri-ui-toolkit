set(QML_PLUGIN_DIR "${QML_PLUGIN_BASE_DIR}/Components/Labs")

add_library(LomiriComponentsLabs MODULE
    plugin/plugin.cpp
)

target_include_directories(LomiriComponentsLabs PRIVATE
    plugin
)

target_link_libraries(LomiriComponentsLabs PRIVATE
    LomiriToolkit
    Qt::Core
    Qt::Gui
    Qt::Qml
    Qt::Quick
)

# install
install(TARGETS LomiriComponentsLabs LIBRARY DESTINATION "${QML_PLUGIN_DIR}")
install(FILES "qmldir" DESTINATION ${QML_PLUGIN_DIR}/)
