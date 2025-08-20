find_package(LTTngUST)

add_executable(lomiri-app-launch-tracepoints
    app-launch-tracepoints.c app-launch-tracepoints.h
)

target_link_libraries(lomiri-app-launch-tracepoints PRIVATE
    LTTng::UST
    Qt::Core
    Qt::Gui
    dl
)

install(
  TARGETS apicheck
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
