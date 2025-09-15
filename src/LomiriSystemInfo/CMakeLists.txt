pkg_check_modules(UDEV REQUIRED libudev)
pkg_check_modules(EVDEV REQUIRED libevdev)
#pkg_check_modules(MIRCLIENT REQUIRED mir1client)

add_library(LomiriSystemInfo SHARED
    lomiriinputinfomanager.cpp
    lomiriinputinfomanager_p.h
    lomiriinputinfo.cpp
    lomiriinputinfo.h

    lomiriscreensaver.cpp
    lomiriscreensaver.h

    #linux/lomiriinputinfomanagermir.cpp
    #linux/lomiriinputinfomanagermir_p.h

    linux/lomiriinputinfomanagerudev.cpp
    linux/lomiriinputinfomanagerudev_p.h

    linux/lomiriscreensaver_mir.cpp
    linux/lomiriscreensaver_mir_p.h

    #linux/qscreensaver_x11.cpp
    #linux/qscreensaver_x11_p.h
)
add_library(LomiriSystemInfo::LomiriSystemInfo ALIAS LomiriSystemInfo)

set_target_properties(LomiriSystemInfo PROPERTIES
    VERSION     ${LOMIRI_UI_TOOLKIT_VERSION}
    SOVERSION   ${LOMIRI_UI_TOOLKIT_SOVERSION}
    OUTPUT_NAME "LomiriSystemInfo-Qt6"
    EXPORT_NAME "LomiriSystemInfo-Qt6"
)

target_include_directories(LomiriSystemInfo
    PUBLIC
        #${MIRCLIENT_INCLUDE_DIRS}
        ${EVDEV_INCLUDE_DIRS}
)

target_link_libraries(LomiriSystemInfo
    Qt::Core
    Qt::Network
    Qt::Gui
    # qpa
    Qt::GuiPrivate
    Qt::DBus

    ${UDEV_LIBRARIES}
    ${EVDEV_LIBRARIES}
    #${MIRCLIENT_LIBRARIES}
)

# define QT_UNITY8
target_compile_definitions(LomiriSystemInfo
    PRIVATE
        QT_UNITY8
        QT_NO_MIR
)

lomiri_target_prepare_headers(LomiriSystemInfo SUBDIRECTORY LomiriSystemInfo-Qt6)

# install
ecm_generate_export_header(LomiriSystemInfo
    VERSION ${LOMIRI_UI_TOOLKIT_VERSION}
    BASE_NAME LomiriSystemInfo
)

ecm_generate_headers(LomiriSystemInfo_FORWARDING_HEADERS
    HEADER_NAMES
        LomiriBatteryInfo
        LomiriInputInfo,LomiriInputDevice,LomiriInputInfoManager
        LomiriDeviceInfo
        LomiriScreenSaver
        LomiriNetworkInfo
    COMMON_HEADER LomiriSystemInfo
)

install(TARGETS LomiriSystemInfo
    EXPORT LomiriSystemInfoTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
)

set(CMAKECONFIG_INSTALL_DIR "${LOMIRI_INSTALL_CMAKEPACKAGEDIR}/LomiriSystemInfo-Qt6")
set(LomiriSystemInfo_VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/lomirisysteminfoversion.h")

ecm_setup_version(
    PROJECT
    VARIABLE_PREFIX LomiriSystemInfo
    VERSION_HEADER "${LomiriSystemInfo_VERSION_HEADER}"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/LomiriSystemInfo-Qt6ConfigVersion.cmake"
    SOVERSION ${LOMIRI_UI_TOOLKIT_SOVERSION}
)

lomiri_target_install_headers(LomiriSystemInfo
    EXTRA_FILES
        ${LomiriSystemInfo_FORWARDING_HEADERS}
        "${LomiriSystemInfo_VERSION_HEADER}"
    DESTINATION "${LOMIRI_INSTALL_INCLUDEDIR}/LomiriSystemInfo-Qt6"
)

ecm_generate_pkgconfig_file(BASE_NAME LomiriSystemInfo-Qt6
    INCLUDE_INSTALL_DIR ${LOMIRI_INSTALL_INCLUDEDIR}/LomiriSystemInfo-Qt6
    DEPS "Qt6Core Qt6Network Qt6Gui Qt6GuiPrivate Qt6DBus libudev libevdev"
    INSTALL
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/LomiriSystemInfo-Qt6Config.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/LomiriSystemInfo-Qt6Config.cmake"
    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
    PATH_VARS CMAKE_INSTALL_PREFIX
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/LomiriSystemInfo-Qt6Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/LomiriSystemInfo-Qt6ConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
)

install(EXPORT LomiriSystemInfoTargets
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    FILE LomiriSystemInfo-Qt6Targets.cmake
    NAMESPACE LomiriSystemInfo::
)
