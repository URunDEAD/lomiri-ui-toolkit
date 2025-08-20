add_executable(createshapetextures
    createshapetextures.cpp
)
target_link_libraries(createshapetextures PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Svg
)

target_compile_options(createshapetextures
    -Wno-unused-variable
)
