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
add_library(LomiriUIToolkit::LomiriMetrics ALIAS LomiriMetrics)

target_compile_definitions(LomiriMetrics PRIVATE
    EGL_NO_X11
    MESA_EGL_NO_X11_HEADERS
)

set_target_properties(LomiriMetrics PROPERTIES
    VERSION     ${LOMIRI_UI_TOOLKIT_VERSION}
    SOVERSION   ${LOMIRI_UI_TOOLKIT_SOVERSION}
    OUTPUT_NAME "LomiriMetrics-Qt6"
    EXPORT_NAME "LomiriMetrics-Qt6"
)

target_link_libraries(LomiriMetrics PRIVATE
    Qt::CorePrivate
    Qt::GuiPrivate
    Qt::QuickPrivate
    dl
    ${EGL_LIBRARIES}
)

ecm_generate_export_header(LomiriMetrics
    VERSION ${LOMIRI_UI_TOOLKIT_VERSION}
    BASE_NAME LomiriMetrics
)

if(LTTngUST_FOUND)
    target_compile_definitions(LomiriMetrics PRIVATE
        LTTNG_PLUGIN_BUILD_PATH=\"${CMAKE_CURRENT_BINARY_DIR}/lttng/libumlttng.so\"
        LTTNG_PLUGIN_INSTALL_PATH=\"${QT_INSTALL_PLUGINS}/lomiri/metrics/libumlttng.so\"
    )

    add_subdirectory(lttng)
endif()

lomiri_target_prepare_headers(LomiriMetrics SUBDIRECTORY LomiriMetrics-Qt6)

# install
install(TARGETS LomiriMetrics
    EXPORT LomiriMetricsTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
)

set(CMAKECONFIG_INSTALL_DIR "${LOMIRI_INSTALL_CMAKEPACKAGEDIR}/LomiriMetrics-Qt6")
set(LomiriMetrics_VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/lomirimetricsversion.h")

ecm_setup_version(
    PROJECT
    VARIABLE_PREFIX LomiriMetrics
    VERSION_HEADER "${LomiriMetrics_VERSION_HEADER}"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/LomiriMetrics-Qt6ConfigVersion.cmake"
    SOVERSION ${LOMIRI_UI_TOOLKIT_SOVERSION}
)

lomiri_target_install_headers(LomiriMetrics
    EXTRA_FILES "${LomiriMetrics_VERSION_HEADER}"
    DESTINATION ${LOMIRI_INSTALL_INCLUDEDIR}/LomiriMetrics-Qt6
)

ecm_generate_pkgconfig_file(BASE_NAME LomiriMetrics-Qt6
    INCLUDE_INSTALL_DIR ${LOMIRI_INSTALL_INCLUDEDIR}/LomiriMetrics-Qt6
    DEPS "Qt6CorePrivate Qt6GuiPrivate Qt6QuickPrivate dl egl"
    INSTALL
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/LomiriMetrics-Qt6Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/LomiriMetrics-Qt6Config.cmake"
    INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
    PATH_VARS CMAKE_INSTALL_PREFIX
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/LomiriMetrics-Qt6Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/LomiriMetrics-Qt6ConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
)

install(EXPORT LomiriMetricsTargets
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    FILE LomiriMetrics-Qt6Targets.cmake
    NAMESPACE LomiriUIToolkit::
)
