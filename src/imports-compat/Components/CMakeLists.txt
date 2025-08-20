cmake_minimum_required(VERSION 3.16)
project(Components VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS DBus Gui Qml Quick Svg)

qt_standard_project_setup()

qt_add_executable(UbuntuComponents WIN32 MACOSX_BUNDLE
    plugin/plugin.cpp plugin/plugin.h
)
target_link_libraries(UbuntuComponents PRIVATE
    LomiriGesturesPrivate
    LomiriToolkitPrivate
    Qt::CorePrivate
    Qt::DBus
    Qt::GuiPrivate
    Qt::QmlPrivate
    Qt::QuickPrivate
    Qt::Svg
)

if((QT_VERSION_MAJOR EQUAL 5) AND (QT_VERSION_MINOR LESS 2))
    target_link_libraries(UbuntuComponents PRIVATE
        v8Private
    )
endif()

install(TARGETS UbuntuComponents
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
    TARGET UbuntuComponents
    FILENAME_VARIABLE deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

add_subdirectory(Labs)
add_subdirectory(ListItems)
add_subdirectory(Pickers)
add_subdirectory(Popups)
add_subdirectory(Styles)
add_subdirectory(Themes)
