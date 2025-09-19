add_library(LomiriTest MODULE
    plugin/testplugin.cpp plugin/testplugin.h
    plugin/uctestcase.cpp plugin/uctestcase.h
    plugin/uctestextras.cpp plugin/uctestextras.h
)

set(QML_FILES
    LomiriTestCase.qml
    LomiriTestCase13.qml
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

install(TARGETS LomiriTest
    DESTINATION "${QML_PLUGIN_BASE_DIR}/Test"
)

install(FILES ${QML_FILES} qmldir
    DESTINATION "${QML_PLUGIN_BASE_DIR}/Test"
)
