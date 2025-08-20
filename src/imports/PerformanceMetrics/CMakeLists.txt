add_library(LomiriPerformanceMetrics SHARED
    plugin/rendertimer.cpp plugin/rendertimer.h
    plugin/upmcpuusage.cpp plugin/upmcpuusage.h
    plugin/upmgraphmodel.cpp plugin/upmgraphmodel.h
    plugin/upmplugin.cpp plugin/upmplugin.h
    plugin/upmrenderingtimes.cpp plugin/upmrenderingtimes.h
    plugin/upmtexturefromimage.cpp plugin/upmtexturefromimage.h
)

target_link_libraries(LomiriPerformanceMetrics PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Qml
    Qt::Quick
)

# install
install(TARGETS LomiriPerformanceMetrics
    LIBRARY DESTINATION "${QML_PLUGIN_BASE_DIR}/PerformanceMetrics"
)

# install qmldir file
install(FILES qmldir PerformanceOverlay.qml BarGraph.qml
    DESTINATION "${QML_PLUGIN_BASE_DIR}/PerformanceMetrics"
)
