add_library(umlttng MODULE
    lttng.c
    tracepoints.c
)

target_link_libraries(umlttng PRIVATE
    LTTng::UST
)

install(TARGETS umlttng
    LIBRARY DESTINATION "${QT_INSTALL_PLUGINS}/lomiri/metrics"
)
