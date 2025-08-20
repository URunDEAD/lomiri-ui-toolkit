add_executable(bitmap-text-builder
    bitmaptextbuilder.cpp
)
target_link_libraries(bitmap-text-builder PRIVATE
    Qt::Core
    Qt::Gui
)