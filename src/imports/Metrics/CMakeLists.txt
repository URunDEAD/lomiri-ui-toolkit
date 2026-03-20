qt_add_qml_module(LomiriMetricsPlugin
    URI Lomiri.Metrics
    VERSION 1.0
    PLUGIN_TARGET LomiriMetricsPlugin
    CLASS_NAME LomiriMetricsPlugin
    OUTPUT_DIRECTORY "${QML_BUILD_DIR}/Lomiri/Metrics"
    NO_GENERATE_EXTRA_QMLDIRS
    NO_GENERATE_QMLTYPES
    NO_GENERATE_PLUGIN_SOURCE
    NO_PLUGIN_OPTIONAL
    SOURCES
        plugin.cpp
)

target_link_libraries(LomiriMetricsPlugin PRIVATE
    LomiriMetrics
    Qt::Core
    Qt::Gui
    Qt::Qml
)

install_qml_module(LomiriMetricsPlugin)
