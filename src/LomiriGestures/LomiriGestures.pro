TARGET = LomiriGestures
QT = core-private gui-private qml-private quick-private

# warnings_are_errors depending on the compiler version, sometimes it doesn't
# add the required flags (for instance for recent clang version on old Qt).
warnings_are_errors:!contains(QMAKE_CXXFLAGS_WARN_ON, -Werror){
    QMAKE_CXXFLAGS_WARN_ON += -Werror
}

HEADERS += \
    $$PWD/candidateinactivitytimer_p.h \
    $$PWD/damper_p.h \
    $$PWD/pool_p.h \
    $$PWD/timer_p.h \
    $$PWD/timesource_p.h \
    $$PWD/touchownershipevent_p.h \
    $$PWD/touchregistry_p.h \
    $$PWD/lomirigesturesglobal.h \
    $$PWD/lomirigesturesmodule.h \
    $$PWD/ucswipearea_p.h \
    $$PWD/ucswipearea_p_p.h \
    $$PWD/unownedtouchevent_p.h

SOURCES += \
    $$PWD/candidateinactivitytimer.cpp \
    $$PWD/damper.cpp \
    $$PWD/timer.cpp \
    $$PWD/timesource.cpp \
    $$PWD/touchownershipevent.cpp \
    $$PWD/touchregistry.cpp \
    $$PWD/lomirigesturesmodule.cpp \
    $$PWD/ucswipearea.cpp \
    $$PWD/unownedtouchevent.cpp

load(lomiri_qt_module)
