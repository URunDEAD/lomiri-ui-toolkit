)
target_link_libraries(SuruGradient PRIVATE
    Qt::Core
    Qt::Gui
)

install(TARGETS SuruGradient
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_generate_deploy_app_script(
    TARGET SuruGradient
    FILENAME_VARIABLE deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
