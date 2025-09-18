add_library(LomiriMetricsPlugin MODULE
    plugin.cpp
)

target_link_libraries(LomiriMetricsPlugin PRIVATE
    LomiriMetrics
    Qt::Core
    Qt::Gui
    Qt::Qml
)

# Install the plugin
install(TARGETS LomiriMetricsPlugin
    DESTINATION "${QML_PLUGIN_BASE_DIR}/Metrics"
)

# install qmldir file
install(FILES qmldir
    DESTINATION "${QML_PLUGIN_BASE_DIR}/Metrics"
)
