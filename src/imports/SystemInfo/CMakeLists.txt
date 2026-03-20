qt_add_qml_module(LomiriSystemInfoPlugin
    URI Lomiri.SystemInfo
    VERSION 1.0
    PLUGIN_TARGET LomiriSystemInfoPlugin
    CLASS_NAME LomiriSystemInfoModule
    OUTPUT_DIRECTORY "${QML_BUILD_DIR}/Lomiri/SystemInfo"
    NO_GENERATE_EXTRA_QMLDIRS
    NO_GENERATE_QMLTYPES
    NO_GENERATE_PLUGIN_SOURCE
    NO_PLUGIN_OPTIONAL
    SOURCES
        lomirideviceinfo.cpp
        lomirideviceinfo_p.h
        lomiriinputdevicemodel.cpp
        lomiriinputdevicemodel_p.h
        lomirinetworkinfo.cpp
        lomirinetworkinfo_p.h
        lomirisysteminfo.cpp
)

target_link_libraries(LomiriSystemInfoPlugin PRIVATE
    LomiriSystemInfo
    Qt::Core
    Qt::Qml
)

install_qml_module(LomiriSystemInfoPlugin)
