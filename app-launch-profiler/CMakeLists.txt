set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(lomiri-app-launch-tracepoints
    app-launch-tracepoints.c app-launch-tracepoints.h
)

set_target_properties(lomiri-app-launch-tracepoints PROPERTIES
    RUNTIME_OUTPUT_NAME "lomiri-app-launch-tracepoints-qt${QT_VERSION_MAJOR}"
)

target_link_libraries(lomiri-app-launch-tracepoints PRIVATE
    LTTng::UST
    Qt::Core
    Qt::Gui
    dl
)

install(
  TARGETS lomiri-app-launch-tracepoints
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
    PROGRAMS lomiri-app-launch-profiler-lttng
    DESTINATION ${CMAKE_INSTALL_BINDIR}
    RENAME "lomiri-app-launch-profiler-lttng-qt${QT_VERSION_MAJOR}"
)
install(
    PROGRAMS lomiri-appstart-profile
    DESTINATION ${CMAKE_INSTALL_BINDIR}
    RENAME "lomiri-appstart-profile-qt${QT_VERSION_MAJOR}"
)
install(
    PROGRAMS lomiri-appstart-test
    DESTINATION ${CMAKE_INSTALL_BINDIR}
    RENAME "lomiri-appstart-test-qt${QT_VERSION_MAJOR}"
)
