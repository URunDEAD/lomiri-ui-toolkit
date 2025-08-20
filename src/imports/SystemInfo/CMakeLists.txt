add_library(LomiriSystemInfoPlugin SHARED
   #lomirideviceinfo.cpp lomirideviceinfo_p.h
   #lomirinetworkinfo.cpp lomirinetworkinfo_p.h
    lomiriinputdevicemodel.cpp lomiriinputdevicemodel_p.h
    qsysteminfo.cpp
)

target_link_libraries(LomiriSystemInfoPlugin PRIVATE
    Qt::Core
    Qt::Qml

    LomiriSystemInfo
)

set(QML_PLUGIN_DIR "${QML_PLUGIN_BASE_DIR}/SystemInfo")

# Install the QML files
install(TARGETS LomiriSystemInfoPlugin
    DESTINATION ${QML_PLUGIN_DIR}
)

install(FILES
    qmldir
    DESTINATION ${QML_PLUGIN_DIR}
)
