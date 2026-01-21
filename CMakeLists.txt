cmake_minimum_required(VERSION 3.16)
project(lomiri-ui-toolkit VERSION 1.3.5903 LANGUAGES C CXX)
cmake_policy(SET CMP0048 NEW) # for ecm_setup_version

option(WITH_LOMIRI_MIR "Enable mir support in LomiriSystemInfo" ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(LOMIRI_UI_TOOLKIT_VERSION "1.0.0")
set(LOMIRI_UI_TOOLKIT_SOVERSION "1")
set(QT_VERSION_MAJOR 6)

find_package(QT NAMES Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS DBus Gui Qml Quick Svg Test Core5Compat)

find_package(Qt6 REQUIRED COMPONENTS ShaderTools)

message(STATUS "Qt6 version ${Qt6Core_VERSION}")

if (Qt6Core_VERSION VERSION_GREATER_EQUAL "6.10.0")
    find_package(Qt6 REQUIRED COMPONENTS CorePrivate GuiPrivate QmlPrivate QuickPrivate)
endif()

find_package(LTTngUST)

find_package(PkgConfig REQUIRED)
pkg_check_modules(XCB REQUIRED xcb)
pkg_check_modules(GLIB REQUIRED glib-2.0)
pkg_check_modules(DBUS REQUIRED dbus-1)
pkg_check_modules(EGL REQUIRED egl)

#pkg_check_modules(QTORGANIZER REQUIRED Qt${QT_VERSION_MAJOR}Organizer)
#pkg_check_modules(QTSYSTEMINFO REQUIRED Qt${QT_VERSION_MAJOR}SystemInfo)

include(GNUInstallDirs)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(ECM "6.13.0" NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(ECMGenerateExportHeader)
include(ECMGenerateHeaders)
include(CMakePackageConfigHelpers)
include(ECMSetupVersion)
include(ECMGeneratePkgConfigFile)

set(QML_PLUGIN_BASE_DIR "${CMAKE_INSTALL_LIBDIR}/qt${QT_VERSION_MAJOR}/qml/Lomiri")
set(QT_INSTALL_PLUGINS "${CMAKE_INSTALL_LIBDIR}/qt${QT_VERSION_MAJOR}/plugins")
set(LOMIRI_INSTALL_INCLUDEDIR "${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_LIBRARY_ARCHITECTURE}")
set(LOMIRI_INSTALL_CMAKEPACKAGEDIR "${CMAKE_INSTALL_LIBDIR}/cmake")

# Carried over from qmake
# FIXME: Fix bug 1606247 and enable the LomiriToolkit namespace.
# build LomiriToolkit without namespace
add_definitions(-DLOMIRITOOLKIT_NO_NAMESPACE)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(LomiriHeaders)

enable_testing()

add_subdirectory(apicheck)
if (LTTngUST_FOUND)
    add_subdirectory(app-launch-profiler)
endif()
add_subdirectory(lomiri-ui-toolkit-launcher)
add_subdirectory(src)
#add_subdirectory(tests)
