add_library(LomiriGestures SHARED
    candidateinactivitytimer.cpp candidateinactivitytimer_p.h
    damper.cpp damper_p.h
    debughelpers.cpp debughelpers_p.h
    lomirigesturesmodule.cpp lomirigesturesmodule.h
    lomirigesturesglobal.h
    pool_p.h
    timer.cpp timer_p.h
    timesource.cpp timesource_p.h
    touchownershipevent.cpp touchownershipevent_p.h
    touchregistry.cpp touchregistry_p.h
    ucswipearea.cpp ucswipearea_p.h
    ucswipearea_p_p.h
    unownedtouchevent.cpp unownedtouchevent_p.h
)
add_library(LomiriUIToolkit::LomiriGestures ALIAS LomiriGestures)

set_target_properties(LomiriGestures PROPERTIES
    VERSION     ${LOMIRI_UI_TOOLKIT_VERSION}
    SOVERSION   ${LOMIRI_UI_TOOLKIT_SOVERSION}
    OUTPUT_NAME "LomiriGestures-Qt6"
    EXPORT_NAME "LomiriGestures-Qt6"
)

target_link_libraries(LomiriGestures PRIVATE
    Qt::CorePrivate
    Qt::GuiPrivate
    Qt::QmlPrivate
    Qt::QuickPrivate
)

lomiri_target_prepare_headers(LomiriGestures SUBDIRECTORY LomiriGestures-Qt6)

ecm_generate_export_header(LomiriGestures
    VERSION ${LOMIRI_UI_TOOLKIT_VERSION}
    BASE_NAME LomiriGestures
)

ecm_generate_headers(LomiriGestures_FORWARDING_HEADERS
    HEADER_NAMES
        LomiriGesturesModule
    COMMON_HEADER LomiriGestures
)

install(TARGETS LomiriGestures
    EXPORT LomiriGesturesTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
)

set(CMAKECONFIG_INSTALL_DIR "${LOMIRI_INSTALL_CMAKEPACKAGEDIR}/LomiriGestures-Qt6")
set(LomiriGestures_VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/lomirigesturesversion.h")

ecm_setup_version(
    PROJECT
    VARIABLE_PREFIX LomiriGestures
    VERSION_HEADER "${LomiriGestures_VERSION_HEADER}"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/LomiriGestures-Qt6ConfigVersion.cmake"
    SOVERSION ${LOMIRI_UI_TOOLKIT_SOVERSION}
)

lomiri_target_install_headers(LomiriGestures
    EXTRA_FILES
        ${LomiriGestures_FORWARDING_HEADERS}
        "${LomiriGestures_VERSION_HEADER}"
    DESTINATION ${LOMIRI_INSTALL_INCLUDEDIR}/LomiriGestures-Qt6
)

ecm_generate_pkgconfig_file(BASE_NAME LomiriGestures-Qt6
    INCLUDE_INSTALL_DIR ${LOMIRI_INSTALL_INCLUDEDIR}/LomiriGestures-Qt6
    DEPS "Qt6CorePrivate Qt6GuiPrivate Qt6QmlPrivate Qt6QuickPrivate"
    INSTALL
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/LomiriGestures-Qt6Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/LomiriGestures-Qt6Config.cmake"
    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
    PATH_VARS CMAKE_INSTALL_PREFIX
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/LomiriGestures-Qt6Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/LomiriGestures-Qt6ConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
)

install(EXPORT LomiriGesturesTargets
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    FILE LomiriGestures-Qt6Targets.cmake
    NAMESPACE LomiriUIToolkit::
)
