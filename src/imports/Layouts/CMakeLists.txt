add_library(LomiriLayouts MODULE
    plugin/propertychanges.cpp plugin/propertychanges_p.h
    plugin/ulconditionallayout.cpp plugin/ulconditionallayout.h plugin/ulconditionallayout_p.h
    plugin/ulitemlayout.cpp plugin/ulitemlayout.h
    plugin/ullayouts.cpp plugin/ullayouts.h plugin/ullayouts_p.h
    plugin/ullayoutsattached.cpp
    plugin/ulplugin.cpp plugin/ulplugin.h
)
target_link_libraries(LomiriLayouts PRIVATE
    Qt::Core
    Qt::CorePrivate
    Qt::Gui
    Qt::GuiPrivate
    Qt::QmlPrivate
    Qt::QuickPrivate
)

# Install the plugin
install(TARGETS LomiriLayouts
    DESTINATION "${QML_PLUGIN_BASE_DIR}/Layouts"
)

# install qmldir file
install(FILES qmldir
    DESTINATION "${QML_PLUGIN_BASE_DIR}/Layouts"
)
