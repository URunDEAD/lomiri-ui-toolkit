add_library(LomiriGestures SHARED
    candidateinactivitytimer.cpp candidateinactivitytimer_p.h
    damper.cpp damper_p.h
    debughelpers.cpp debughelpers_p.h
    lomirigesturesglobal.h
    lomirigesturesmodule.cpp lomirigesturesmodule.h
    pool_p.h
    timer.cpp timer_p.h
    timesource.cpp timesource_p.h
    touchownershipevent.cpp touchownershipevent_p.h
    touchregistry.cpp touchregistry_p.h
    ucswipearea.cpp ucswipearea_p.h
    ucswipearea_p_p.h
    unownedtouchevent.cpp unownedtouchevent_p.h
)
target_link_libraries(LomiriGestures PRIVATE
    Qt::CorePrivate
    Qt::GuiPrivate
    Qt::QmlPrivate
    Qt::QuickPrivate
)

lomiri_target_prepare_includes(LomiriGestures LomiriGestures-Qt6)

# install
install(TARGETS LomiriGestures
    LIBRARY DESTINATION ${CMAKE_INSTALL_FULL_LIBDIR}
)
