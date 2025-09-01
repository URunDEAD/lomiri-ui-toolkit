file(CREATE_LINK ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/../LomiriSystemInfoQt6 SYMBOLIC)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

pkg_check_modules(UDEV REQUIRED libudev)
pkg_check_modules(EVDEV REQUIRED libevdev)
#pkg_check_modules(MIRCLIENT REQUIRED mir1client)

add_library(LomiriSystemInfo SHARED
    lomiriinputinfomanager.cpp
    lomiriinputinfomanager_p.h
    lomiriinputinfo.cpp
    lomiriinputinfo.h

    lomiriscreensaver.cpp
    lomiriscreensaver.h

    #linux/lomiriinputinfomanagermir.cpp
    #linux/lomiriinputinfomanagermir_p.h

    linux/lomiriinputinfomanagerudev.cpp
    linux/lomiriinputinfomanagerudev_p.h

    linux/lomiriscreensaver_mir.cpp
    linux/lomiriscreensaver_mir_p.h

    #linux/qscreensaver_x11.cpp
    #linux/qscreensaver_x11_p.h
)

target_include_directories(LomiriSystemInfo
    PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}/..
        #${MIRCLIENT_INCLUDE_DIRS}
        ${EVDEV_INCLUDE_DIRS}
)

target_link_libraries(LomiriSystemInfo
    Qt::Core
    Qt::Network
    Qt::Gui
    # qpa
    Qt::GuiPrivate
    Qt::DBus

    ${UDEV_LIBRARIES}
    ${EVDEV_LIBRARIES}
    #${MIRCLIENT_LIBRARIES}
)

# define QT_UNITY8
target_compile_definitions(LomiriSystemInfo
    PRIVATE
        QT_UNITY8
        QT_NO_MIR
)

# install
install(TARGETS LomiriSystemInfo
    LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
)
