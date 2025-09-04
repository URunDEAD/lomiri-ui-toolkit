add_library(LomiriComponents SHARED
    plugin/plugin.cpp
)

target_link_libraries(LomiriComponents PRIVATE
    LomiriToolkit
    Qt::Core
    Qt::CorePrivate
    Qt::Gui
    Qt::GuiPrivate
    Qt::Qml
    Qt::Quick
)

qt6_add_shaders(LomiriComponents "LomiriComponents_shaders"
    PRECOMPILE
    OPTIMIZED
    FILES
        "1.0/shaders/colorizeimage.frag"
        "1.2/shaders/colourshader.frag"
        "Themes/Ambiance/shaders/magnifier.frag"
        "Themes/Ambiance/shaders/partialcolorize.frag"
)

set(QML_PLUGIN_DIR "${QML_PLUGIN_BASE_DIR}/Components")

# Install the QML files
install(TARGETS LomiriComponents
    DESTINATION ${QML_PLUGIN_DIR}
)

file(GLOB ARTWORK_FILES "artwork/*")

set(QML_FILES
    "2.0/ActionBar.qml"
    "2.0/ActivityIndicator.qml"
    "2.0/AdaptivePageLayout.qml"
    "2.0/AnimatedItem.qml"
    "2.0/AppHeader.qml"
    "2.0/Button.qml"
    "2.0/Captions.qml"
    "2.0/CheckBox.qml"
    "2.0/ComboButton.qml"
    "2.0/CrossFadeImage.qml"
    "2.0/dateUtils.js"
    "2.0/DraggingArea.qml"
    "2.0/InputHandler.qml"
    "2.0/MainView.qml"
    "2.0/OptionSelectorDelegate.qml"
    "2.0/OptionSelector.qml"
    "2.0/OrientationHelper.qml"
    "2.0/PageHeadConfiguration.qml"
    "2.0/PageHeadSections.qml"
    "2.0/PageHeadState.qml"
    "2.0/Page.qml"
    "2.0/PageStack.qml"
    "2.0/pageUtils.js"
    "2.0/Panel.qml"
    "2.0/ProgressBar.qml"
    "2.0/PullToRefresh.qml"
    "2.0/Scrollbar.qml"
    "2.0/Sections.qml"
    "2.0/Slider.qml"
    "2.0/Switch.qml"
    "2.0/TabBar.qml"
    "2.0/Tab.qml"
    "2.0/Tabs.qml"
    "2.0/TextArea.qml"
    "2.0/TextCursor.qml"
    "2.0/TextField.qml"
    "2.0/TextInputPopover.qml"
    "2.0/Toolbar.qml"
    "2.0/ToolbarButton.qml"
    "2.0/ToolbarItems.qml"
    "2.0/LomiriColors.qml"
    "2.0/LomiriListView.qml"
    "2.0/LomiriNumberAnimation.qml"
    "2.0/ListItemPopover.qml"
    "2.0/PageColumn.qml"
    "2.0/PageColumnsLayout.qml"
    "2.0/ProgressionSlot.qml"
    "2.0/ScrollView.qml"
    "2.0/PageHeader.qml"
    "2.0/Haptics.qml"
    "2.0/Icon.qml"
    "2.0/AbstractButton.qml"
    "2.0/ActionList.qml"
    "2.0/Label.qml"
    "2.0/MainViewBase.qml"
    "2.0/PageTreeNode.qml"
    "2.0/PageWrapper.qml"
)

install(FILES ${ARTWORK_FILES} DESTINATION ${QML_PLUGIN_DIR}/artwork)
install(FILES ${QML_FILES} DESTINATION ${QML_PLUGIN_DIR}/2.0)
install(FILES "qmldir-qt6" DESTINATION ${QML_PLUGIN_DIR}/ RENAME "qmldir")
    

add_subdirectory(Labs)
add_subdirectory(ListItems)
add_subdirectory(Pickers)
add_subdirectory(Popups)
add_subdirectory(Styles)
add_subdirectory(Themes)
