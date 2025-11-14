add_executable(lomiri-ui-toolkit-launcher
    launcher.cpp
)

set_target_properties(lomiri-ui-toolkit-launcher PROPERTIES
    RUNTIME_OUTPUT_NAME "lomiri-ui-toolkit-launcher-qt${QT_VERSION_MAJOR}"
)

target_link_libraries(lomiri-ui-toolkit-launcher PRIVATE
    Qt::CorePrivate
    Qt::GuiPrivate
    Qt::QuickPrivate
    Qt::Qml
    Qt::Test
    LomiriToolkit
    LomiriMetrics
)

install(
  TARGETS lomiri-ui-toolkit-launcher
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
