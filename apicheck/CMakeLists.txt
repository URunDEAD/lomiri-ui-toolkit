add_executable(apicheck
    apicheck.cpp
)

set_target_properties(apicheck PROPERTIES
    RUNTIME_OUTPUT_NAME "apicheck-qt${QT_VERSION_MAJOR}"
)

target_link_libraries(apicheck PRIVATE
    Qt::Core
    Qt::CorePrivate
    Qt::Gui
    Qt::GuiPrivate
    Qt::Qml
    Qt::QmlPrivate
    Qt::Quick
    Qt::QuickPrivate
    Qt::Test
)

install(
  TARGETS apicheck
  RUNTIME DESTINATION ${CMAKE_INSTALL_LIBEXECDIR}/lomiri-ui-toolkit
)
