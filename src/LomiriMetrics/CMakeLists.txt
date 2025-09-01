file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/../LomiriMetricsQt6 SYMBOLIC)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(QML_PLUGIN_DIR "${QML_PLUGIN_BASE_DIR}/PerformanceMetrics")

add_library(LomiriMetrics SHARED
    applicationmonitor.cpp applicationmonitor.h applicationmonitor_p.h
    bitmaptext.cpp bitmaptext_p.h
    bitmaptextfont_p.h
    events.cpp events.h events_p.h
    gputimer.cpp gputimer_p.h
    logger.cpp logger.h logger_p.h
    lomirimetricsglobal.cpp lomirimetricsglobal.h lomirimetricsglobal_p.h
    overlay.cpp overlay_p.h
)
target_compile_definitions(LomiriMetrics PRIVATE
    EGL_NO_X11
    MESA_EGL_NO_X11_HEADERS
)

target_link_libraries(LomiriMetrics PRIVATE
    Qt::CorePrivate
    Qt::GuiPrivate
    Qt::QuickPrivate
    dl
    ${EGL_LIBRARIES}
)

if(LINUX AND LTTngUST_FOUND)
    target_compile_definitions(LomiriMetrics PRIVATE
        LTTNG_PLUGIN_BUILD_PATH=\"${CMAKE_CURRENT_BINARY_DIR}/lttng/libumlttng.so\"
        LTTNG_PLUGIN_INSTALL_PATH=\"$$[QT_INSTALL_PLUGINS]/lomiri/metrics/libumlttng.so\"
    )

    add_subdirectory(lttng)
endif()

target_include_directories(LomiriMetrics PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/..)

# install
install(TARGETS LomiriMetrics
    LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
)
