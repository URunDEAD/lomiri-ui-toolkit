set(QML_PLUGIN_DIR "${CMAKE_INSTALL_LIBDIR}/qt${QT_VERSION_MAJOR}/qml/Lomiri/Components")

if(NOT (TEST_architecture_arch STREQUAL "arm") AND FALSE)
    pkg_check_modules(X11 REQUIRED x11)
    pkg_check_modules(XCB REQUIRED xcb)
    pkg_check_modules(Xi REQUIRED xi)

    set(MOUSE_TOUCH_ADAPTOR_SOURCES
        mousetouchadaptor_x11.cpp
    )

    set(MOUSE_TOUCH_ADAPTOR_DEFINES
        LOMIRITOOLKIT_ENABLE_X11_TOUCH_EMULATION
    )

    set(MOUSE_TOUCH_ADAPTOR_LIBS
        ${X11_LIBRARIES}
        ${XCB_LIBRARIES}
        ${Xi_LIBRARIES}
    )
endif()

add_library(LomiriToolkit SHARED
    ${MOUSE_TOUCH_ADAPTOR_SOURCES}
    actionlist.cpp actionlist_p.h
    adapters/actionsproxy_p.cpp adapters/actionsproxy_p.h
    #adapters/alarmsadapter_p.h
    #adapters/alarmsadapter_organizer.cpp
    adapters/dbuspropertywatcher_p.cpp adapters/dbuspropertywatcher_p.h
    asyncloader.cpp asyncloader_p.h
    asyncloader_p_p.h
    colorutils.cpp colorutils_p.h
    exclusivegroup.cpp exclusivegroup_p.h
    filterbehavior.cpp filterbehavior_p.h
    i18n.cpp i18n_p.h
    inversemouseareatype.cpp inversemouseareatype_p.h
    label_p.h
    listener.cpp listener_p.h
    livetimer.cpp livetimer_p.cpp livetimer_p.h
    livetimer_p_p.h
    lomirithemeiconprovider.cpp lomirithemeiconprovider_p.h
    lomiritoolkitglobal.h
    lomiritoolkitmodule.cpp lomiritoolkitmodule.h
    menu.cpp menu_p.h
    menu_p_p.h
    menubar.cpp menubar_p.h
    menubar_p_p.h
    menugroup.cpp menugroup_p.h
    mousetouchadaptor.cpp mousetouchadaptor_p.h
    mousetouchadaptor_p_p.h
    privates/appheaderbase.cpp privates/appheaderbase_p.h
    privates/frame.cpp privates/frame_p.h
    privates/listitemdragarea.cpp privates/listitemdragarea_p.h
    privates/listitemdraghandler.cpp privates/listitemdraghandler_p.h
    privates/listitemexpansion.cpp
    privates/listitemselection.cpp privates/listitemselection_p.h
    privates/listviewextensions.cpp privates/listviewextensions_p.h
    privates/splitviewhandler.cpp privates/splitviewhandler_p.h
    privates/threelabelsslot_p.cpp privates/threelabelsslot_p.h
    privates/ucpagewrapper.cpp privates/ucpagewrapper_p.h
    privates/ucpagewrapper_p_p.h
    privates/ucpagewrapperincubator.cpp privates/ucpagewrapperincubator_p.h
    privates/ucscrollbarutils.cpp privates/ucscrollbarutils_p.h
    propertychange.cpp propertychange_p.h
    qquickclipboard.cpp qquickclipboard_p.h
    qquickclipboard_p_p.h
    qquickmimedata.cpp qquickmimedata_p.h
    quickutils.cpp quickutils_p.h
    sortbehavior.cpp sortbehavior_p.h
    sortfiltermodel.cpp sortfiltermodel_p.h
    splitview.cpp splitview_p.h
    splitview_p_p.h
    splitviewlayout.cpp
    statesaverbackend_p.cpp statesaverbackend_p.h
    timeutils_p.h
    tree.cpp tree_p.h
    ucabstractbutton.cpp ucabstractbutton_p.h
    ucabstractbutton_p_p.h
    ucaction.cpp ucaction_p.h
    ucactioncontext.cpp ucactioncontext_p.h
    ucactionitem.cpp ucactionitem_p.h
    ucactionitem_p_p.h
    ucactionmanager.cpp ucactionmanager_p.h
    ucapplication.cpp ucapplication_p.h
    ucargument.cpp ucargument_p.h
    ucarguments.cpp ucarguments_p.h
    ucbottomedge.cpp ucbottomedge_p.h
    ucbottomedge_p_p.h
    ucbottomedgehint.cpp ucbottomedgehint_p.h
    ucbottomedgehint_p_p.h
    ucbottomedgeregion.cpp ucbottomedgeregion_p.h
    ucbottomedgeregion_p_p.h
    ucbottomedgestyle.cpp ucbottomedgestyle_p.h
    ucdefaulttheme.cpp ucdefaulttheme_p.h
    ucdeprecatedtheme.cpp ucdeprecatedtheme_p.h
    ucfontutils.cpp ucfontutils_p.h
    uchaptics.cpp uchaptics_p.h
    ucheader.cpp ucheader_p.h
    ucimportversionchecker_p.cpp ucimportversionchecker_p.h
    ucinversemouse_p.h
    uclabel.cpp uclabel_p.h
    uclistitem.cpp uclistitem_p.h
    uclistitem_p_p.h
    uclistitemactions.cpp uclistitemactions_p.h
    uclistitemactions_p_p.h
    uclistitemlayout.cpp uclistitemlayout_p.h
    uclistitemstyle.cpp uclistitemstyle_p.h
    uclomirianimation.cpp uclomirianimation_p.h
    uclomirishape.cpp uclomirishape_p.h
    uclomirishapeoverlay.cpp uclomirishapeoverlay_p.h
    uclomirishapetextures.cpp uclomirishapetextures_p.h
    ucmainviewbase.cpp ucmainviewbase_p.h
    ucmainviewbase_p_p.h
    ucmainwindow.cpp ucmainwindow_p.h
    ucmainwindow_p_p.h
    ucmargins_p.h
    ucmathutils.cpp ucmathutils_p.h
    ucmouse_p.h
    ucmousefilters.cpp
    ucnamespace.cpp ucnamespace_p.h
    ucpagetreenode.cpp ucpagetreenode_p.h
    ucpagetreenode_p_p.h
    ucperformancemonitor.cpp ucperformancemonitor_p.h
    ucproportionalshape.cpp ucproportionalshape_p.h
    ucqquickimageextension.cpp ucqquickimageextension_p.h
    ucscalingimageprovider.cpp ucscalingimageprovider_p.h
    ucserviceproperties.cpp ucserviceproperties_p.h
    ucserviceproperties_p_p.h
    ucslotslayout.cpp ucslotslayout_p.h
    ucslotslayout_p_p.h
    ucstatesaver.cpp ucstatesaver_p.h
    ucstatesaver_p_p.h
    ucstyleditembase.cpp ucstyleditembase_p.h
    ucstyleditembase_p_p.h
    ucstylehints.cpp ucstylehints_p.h
    uctheme.cpp uctheme_p.h
    ucthemingextension.cpp ucthemingextension_p.h
    ucunits.cpp ucunits_p.h
    ucurihandler.cpp ucurihandler_p.h
    ucviewitemsattached.cpp
    unixsignalhandler_p.cpp unixsignalhandler_p.h
)

# -fpermissive
target_compile_options(LomiriToolkit PRIVATE
    -fpermissive
)

target_link_libraries(LomiriToolkit PRIVATE
    LomiriGestures
    LomiriMetrics
    Qt::CorePrivate
    Qt::DBus
    Qt::GuiPrivate
    Qt::QmlPrivate
    Qt::QuickPrivate
    Qt::Svg
    Qt::Test
 #   Qt::Organizer
 #   Qt::SystemInfo
 #   Qt::SystemInfoPrivate
    ${QT5COMPAT}

    LomiriSystemInfo

    ${DBUS_LIBRARIES}
    ${GLIB_LIBRARIES}
    ${MOUSE_TOUCH_ADAPTOR_LIBS}
)

target_include_directories(LomiriToolkit
    PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}/..
    PRIVATE
        # glib
        ${GLIB_INCLUDE_DIRS}
)

target_compile_definitions(LomiriToolkit PRIVATE
    ${MOUSE_TOUCH_ADAPTOR_DEFINES}
)

lomiri_target_prepare_includes(LomiriToolkit LomiriToolkit-Qt6)

# Resources:
set(resources_resource_files
    "privates/shaders/frame.frag"
    "privates/shaders/frame.vert"
    "shaders/shape.frag"
    "shaders/shape.vert"
    "shaders/shape_mipmap.frag"
    "shaders/shapeoverlay.frag"
    "shaders/shapeoverlay.vert"
    "shaders/shapeoverlay_mipmap.frag"
)

# install
install(TARGETS LomiriToolkit
    LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
)
