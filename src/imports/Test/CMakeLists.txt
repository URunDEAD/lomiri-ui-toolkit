add_library(LomiriTest MODULE
    plugin/testplugin.cpp plugin/testplugin.h
    plugin/uctestcase.cpp plugin/uctestcase.h
    plugin/uctestextras.cpp plugin/uctestextras.h
)

target_compile_definitions(LomiriTest PRIVATE
    LOMIRI_QML_IMPORT_PATH="/qml"
)

target_link_libraries(LomiriTest PRIVATE
    LomiriGestures
    LomiriToolkit
    Qt::Core
    Qt::CorePrivate
    Qt::Gui
    Qt::GuiPrivate
    Qt::Qml
    Qt::QmlPrivate
    Qt::QuickPrivate
    Qt::Test
)
