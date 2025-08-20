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

set(QML_PLUGIN_DIR "${QML_PLUGIN_BASE_DIR}/Components")

# Install the QML files
install(TARGETS LomiriComponents
    DESTINATION ${QML_PLUGIN_DIR}
)

file(GLOB ARTWORK_FILES "artwork/*")

set(QML_FILES_1_0
    "1.0/Button.qml"
    "1.0/Icon.qml"
    "1.0/ProgressBar.qml"
    "1.0/LomiriColors.qml"
)

set(QML_FILES_1_1
    ${QML_FILES_1_0}
    "1.1/Button.qml"
    "1.1/Haptics.qml"
    "1.1/Icon.qml"
    "1.1/ProgressBar.qml"
    "1.1/LomiriColors.qml"
)

set(QML_FILES_1_2
    ${QML_FILES_1_1}
    "1.2/AbstractButton.qml"
    "1.2/ActionList.qml"
    "1.2/ActivityIndicator.qml"
    "1.2/AnimatedItem.qml"
    "1.2/AppHeader.qml"
    "1.2/Captions.qml"
    "1.2/CheckBox.qml"
    "1.2/ComboButton.qml"
    "1.2/CrossFadeImage10.qml"
    "1.2/CrossFadeImage11.qml"
    "1.2/dateUtils.js"
    "1.2/DraggingArea.qml"
    "1.2/Header.qml"
    "1.2/InputHandler.qml"
    "1.2/Label.qml"
    "1.2/MainView12.qml"
    "1.2/MainViewBase.qml"
    "1.2/MainView.qml"
    "1.2/OptionSelectorDelegate.qml"
    "1.2/OptionSelector.qml"
    "1.2/OrientationHelper.qml"
    "1.2/Page10.qml"
    "1.2/Page11.qml"
    "1.2/PageHeadConfiguration.qml"
    "1.2/PageHeadSections.qml"
    "1.2/PageHeadState.qml"
    "1.2/PageStack.qml"
    "1.2/PageTreeNode.qml"
    "1.2/PageWrapper.qml"
    "1.2/PageWrapperUtils.js"
    "1.2/Panel.qml"
    "1.2/PullToRefresh.qml"
    "1.2/Scrollbar.qml"
    "1.2/Slider.qml"
    "1.2/sliderUtils.js"
    "1.2/stack.js"
    "1.2/Switch.qml"
    "1.2/TabBar.qml"
    "1.2/Tab.qml"
    "1.2/Tabs.qml"
    "1.2/TextArea.qml"
    "1.2/TextCursor.qml"
    "1.2/TextField.qml"
    "1.2/TextInputPopover.qml"
    "1.2/ToolbarButton.qml"
    "1.2/ToolbarItems.qml"
    "1.2/Toolbar.qml"
    "1.2/LomiriListView11.qml"
    "1.2/LomiriListView.qml"
    "1.2/LomiriNumberAnimation.qml"
)

set(QML_FILES_1_3
    ${QML_FILES_1_2}
    "1.2/AbstractButton.qml"
    "1.3/ActionBar.qml"
    "1.3/ActivityIndicator.qml"
    "1.3/AdaptivePageLayout.qml"
    "1.3/AnimatedItem.qml"
    "1.3/AppHeader.qml"
    "1.3/Button.qml"
    "1.3/Captions.qml"
    "1.3/CheckBox.qml"
    "1.3/ComboButton.qml"
    "1.3/CrossFadeImage.qml"
    "1.3/dateUtils.js"
    "1.3/DraggingArea.qml"
    "1.3/InputHandler.qml"
    "1.3/MainView.qml"
    "1.3/OptionSelectorDelegate.qml"
    "1.3/OptionSelector.qml"
    "1.3/OrientationHelper.qml"
    "1.3/PageHeadConfiguration.qml"
    "1.3/PageHeadSections.qml"
    "1.3/PageHeadState.qml"
    "1.3/Page.qml"
    "1.3/PageStack.qml"
    "1.3/pageUtils.js"
    "1.3/Panel.qml"
    "1.3/ProgressBar.qml"
    "1.3/PullToRefresh.qml"
    "1.3/Scrollbar.qml"
    "1.3/Sections.qml"
    "1.3/Slider.qml"
    "1.3/Switch.qml"
    "1.3/TabBar.qml"
    "1.3/Tab.qml"
    "1.3/Tabs.qml"
    "1.3/TextArea.qml"
    "1.3/TextCursor.qml"
    "1.3/TextField.qml"
    "1.3/TextInputPopover.qml"
    "1.3/Toolbar.qml"
    "1.3/ToolbarButton.qml"
    "1.3/ToolbarItems.qml"
    "1.3/LomiriColors.qml"
    "1.3/LomiriListView.qml"
    "1.3/LomiriNumberAnimation.qml"
    "1.3/ListItemPopover.qml"
    "1.3/PageColumn.qml"
    "1.3/PageColumnsLayout.qml"
    "1.3/ProgressionSlot.qml"
    "1.3/ScrollView.qml"
    "1.3/PageHeader.qml"
    "1.3/Icon.qml"

    "1.3/CrossFadeImage.qdoc"
    "1.3/PageHeadConfiguration.qdoc"
    "1.3/MainView.qdoc"
    #"1.3/Icon.qdoc"
    "1.3/dateUtils.js"
    "1.3/pageUtils.js"
)

set(OTHER_FILES
    "qmldir"
)

# Combine all QML files
set(ALL_QML_FILES
    ${ARTWORK_FILES}
    ${QML_FILES_1_0}
    ${QML_FILES_1_1}
    ${QML_FILES_1_2}
    ${QML_FILES_1_3}
    ${OTHER_FILES}
)

install(FILES ${ARTWORK_FILES}
    DESTINATION ${QML_PLUGIN_DIR}/artwork
)

install(FILES ${QML_FILES_1_0}
    DESTINATION ${QML_PLUGIN_DIR}/1.0
)

install(FILES ${QML_FILES_1_1}
    DESTINATION ${QML_PLUGIN_DIR}/1.1
)

install(FILES ${QML_FILES_1_2}
    DESTINATION ${QML_PLUGIN_DIR}/1.2
)

install(FILES ${QML_FILES_1_3}
    DESTINATION ${QML_PLUGIN_DIR}/1.3
)

install(FILES ${OTHER_FILES}
    DESTINATION ${QML_PLUGIN_DIR}
)

add_subdirectory(Labs)
add_subdirectory(ListItems)
add_subdirectory(Pickers)
add_subdirectory(Popups)
add_subdirectory(Styles)
add_subdirectory(Themes)
