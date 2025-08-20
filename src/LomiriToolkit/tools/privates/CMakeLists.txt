add_executable(createprivateshapetextures
    createprivateshapetextures.cpp
)
target_link_libraries(createprivateshapetextures PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Svg
)

target_compile_options(createprivateshapetextures
    -Wno-unused-variable
)