cmake_minimum_required(VERSION 3.16)
project(lomiri-sdk VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
option(ENABLE_QT6 "Enable building with Qt6" ON)

if (ENABLE_QT6)
    set(QT_VERSION_MAJOR 6)
else()
    set(QT_VERSION_MAJOR 5)
endif()

find_package(QT NAMES Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS DBus Gui Qml Quick Svg Test)

# if qt6 also include qt5compat
set(QT5COMPAT "")
if (QT_VERSION_MAJOR EQUAL 6)
message("Building with Qt6")
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core5Compat)
    set(QT5COMPAT Qt${QT_VERSION_MAJOR}::Core5Compat)
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

set(QML_PLUGIN_BASE_DIR "${CMAKE_INSTALL_LIBDIR}/qt${QT_VERSION_MAJOR}/qml/Lomiri")

# Carried over from qmake
# FIXME: Fix bug 1606247 and enable the LomiriToolkit namespace.
# build LomiriToolkit without namespace
add_definitions(-DLOMIRITOOLKIT_NO_NAMESPACE)

enable_testing()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(LomiriTargetPrepareIncludes)

#add_subdirectory(apicheck)
#add_subdirectory(app-launch-profiler)
add_subdirectory(src)
#add_subdirectory(tests)
