qt_add_qml_module(LomiriPerformanceMetrics
    URI Lomiri.PerformanceMetrics
    VERSION 1.0
    PLUGIN_TARGET LomiriPerformanceMetrics
    CLASS_NAME LomiriPerformanceMetricsPlugin
    OUTPUT_DIRECTORY "${QML_BUILD_DIR}/Lomiri/PerformanceMetrics"
    NO_GENERATE_EXTRA_QMLDIRS
    NO_GENERATE_QMLTYPES
    NO_GENERATE_PLUGIN_SOURCE
    NO_PLUGIN_OPTIONAL
    SOURCES
        plugin/rendertimer.cpp
        plugin/rendertimer.h
        plugin/upmcpuusage.cpp
        plugin/upmcpuusage.h
        plugin/upmgraphmodel.cpp
        plugin/upmgraphmodel.h
        plugin/upmplugin.cpp
        plugin/upmplugin.h
        plugin/upmrenderingtimes.cpp
        plugin/upmrenderingtimes.h
        plugin/upmtexturefromimage.cpp
        plugin/upmtexturefromimage.h
    QML_FILES
        BarGraph.qml
        PerformanceOverlay.qml
)

target_link_libraries(LomiriPerformanceMetrics PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Qml
    Qt::Quick
)

install_qml_module(LomiriPerformanceMetrics)
