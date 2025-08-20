add_executable(apicheck
    apicheck.cpp
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
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
