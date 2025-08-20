qt_add_executable(UbuntuComponentsStyles WIN32 MACOSX_BUNDLE
    plugin/plugin.cpp
)
qt_add_qml_module(UbuntuComponentsStyles
    URI UbuntuComponentsStyles
    VERSION 1.1
)

# Avoid collision between executable name and QML module directory (QTBUG-98568).
if(NOT WIN32 AND NOT APPLE)
    set_property(TARGET UbuntuComponentsStyles PROPERTY OUTPUT_NAME UbuntuComponentsStyles_app)
endif()

target_include_directories(UbuntuComponentsStyles PRIVATE
    plugin
)

target_link_libraries(UbuntuComponentsStyles PRIVATE
    LomiriToolkit
    LomiriToolkit_private
    Qt::Core
    Qt::Gui
    Qt::Qml
    Qt::Quick
)

install(TARGETS UbuntuComponentsStyles
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_qml_app_script(
    TARGET UbuntuComponentsStyles
    FILENAME_VARIABLE deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
    DEPLOY_USER_QML_MODULES_ON_UNSUPPORTED_PLATFORM
    MACOS_BUNDLE_POST_BUILD
)
install(SCRIPT ${deploy_script})
