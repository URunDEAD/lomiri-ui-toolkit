cmake_minimum_required(VERSION 3.16)
project(UbuntuComponentsLabs VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui Qml Quick)

qt_standard_project_setup()

qt_add_executable(UbuntuComponentsLabs WIN32 MACOSX_BUNDLE
    plugin/plugin.cpp
)
qt_add_qml_module(UbuntuComponentsLabs
    URI UbuntuComponentsLabs
    VERSION 1.0
)

# Avoid collision between executable name and QML module directory (QTBUG-98568).
if(NOT WIN32 AND NOT APPLE)
    set_property(TARGET UbuntuComponentsLabs PROPERTY OUTPUT_NAME UbuntuComponentsLabs_app)
endif()

target_include_directories(UbuntuComponentsLabs PRIVATE
    plugin
)

target_link_libraries(UbuntuComponentsLabs PRIVATE
    LomiriToolkit
    LomiriToolkit_private
    Qt::Core
    Qt::Gui
    Qt::Qml
    Qt::Quick
)

install(TARGETS UbuntuComponentsLabs
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_qml_app_script(
    TARGET UbuntuComponentsLabs
    FILENAME_VARIABLE deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
    DEPLOY_USER_QML_MODULES_ON_UNSUPPORTED_PLATFORM
    MACOS_BUNDLE_POST_BUILD
)
install(SCRIPT ${deploy_script})
