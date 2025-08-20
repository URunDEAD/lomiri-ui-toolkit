
add_library(UbuntuMetrics SHARED
    plugin.cpp
)

target_link_libraries(UbuntuMetrics PRIVATE
    LomiriMetrics
    LomiriToolkitPrivate
    Qt::Core
    Qt::Gui
    Qt::Qml
)