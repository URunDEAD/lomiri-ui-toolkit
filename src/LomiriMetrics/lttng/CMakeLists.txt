add_library(umlttng SHARED
    lttng.c
    tracepoints.c
)
target_link_libraries(umlttng PRIVATE
    LTTng::UST
)