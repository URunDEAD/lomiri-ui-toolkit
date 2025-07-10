CXX_MODULE = qml
TARGET  = LomiriComponents
TARGETPATH = Lomiri/Components
equals(QT_MAJOR_VERSION, 5) {
    IMPORT_VERSION = 0.1
} else {
    IMPORT_VERSION = 2.0
}

include(plugin/plugin.pri)

#artwork files
ARTWORK_FILES = $$system(find artwork -type f)

QML_FILES += $$ARTWORK_FILES

equals(QT_MAJOR_VERSION, 5) {
#1.0
    QML_FILES += 1.0/Button.qml \
                 1.0/Icon.qml \
                 1.0/ProgressBar.qml \
                 1.0/LomiriColors.qml

#1.1
    QML_FILES += 1.1/Button.qml \
                 1.1/Haptics.qml \
                 1.1/Icon.qml \
                 1.1/ProgressBar.qml \
                 1.1/LomiriColors.qml

#1.2
    QML_FILES += 1.2/AbstractButton.qml \
                 1.2/ActionList.qml \
                 1.2/ActivityIndicator.qml \
                 1.2/AnimatedItem.qml \
                 1.2/AppHeader.qml \
                 1.2/Captions.qml \
                 1.2/CheckBox.qml \
                 1.2/ComboButton.qml \
                 1.2/CrossFadeImage10.qml \
                 1.2/CrossFadeImage11.qml \
                 1.2/dateUtils.js \
                 1.2/DraggingArea.qml \
                 1.2/Header.qml \
                 1.2/InputHandler.qml \
                 1.2/Label.qml \
                 1.2/MainView12.qml \
                 1.2/MainViewBase.qml \
                 1.2/MainView.qml \
                 1.2/OptionSelectorDelegate.qml \
                 1.2/OptionSelector.qml \
                 1.2/OrientationHelper.qml \
                 1.2/Page10.qml \
                 1.2/Page11.qml \
                 1.2/PageHeadConfiguration.qml \
                 1.2/PageHeadSections.qml \
                 1.2/PageHeadState.qml \
                 1.2/PageStack.qml \
                 1.2/PageTreeNode.qml \
                 1.2/PageWrapper.qml \
                 1.2/PageWrapperUtils.js \
                 1.2/Panel.qml \
                 1.2/PullToRefresh.qml \
                 1.2/Scrollbar.qml \
                 1.2/Slider.qml \
                 1.2/sliderUtils.js \
                 1.2/stack.js \
                 1.2/Switch.qml \
                 1.2/TabBar.qml \
                 1.2/Tab.qml \
                 1.2/Tabs.qml \
                 1.2/TextArea.qml \
                 1.2/TextCursor.qml \
                 1.2/TextField.qml \
                 1.2/TextInputPopover.qml \
                 1.2/ToolbarButton.qml \
                 1.2/ToolbarItems.qml \
                 1.2/Toolbar.qml \
                 1.2/LomiriListView11.qml \
                 1.2/LomiriListView.qml \
                 1.2/LomiriNumberAnimation.qml

#1.3
    QML_FILES += 1.3/ActionBar.qml \
                 1.3/ActivityIndicator.qml \
                 1.3/AdaptivePageLayout.qml \
                 1.3/AnimatedItem.qml \
                 1.3/AppHeader.qml \
                 1.3/Button.qml \
                 1.3/Captions.qml \
                 1.3/CheckBox.qml \
                 1.3/ComboButton.qml \
                 1.3/CrossFadeImage.qml \
                 1.3/dateUtils.js \
                 1.3/DraggingArea.qml \
                 1.3/InputHandler.qml \
                 1.3/MainView.qml \
                 1.3/OptionSelectorDelegate.qml \
                 1.3/OptionSelector.qml \
                 1.3/OrientationHelper.qml \
                 1.3/PageHeadConfiguration.qml \
                 1.3/PageHeadSections.qml \
                 1.3/PageHeadState.qml \
                 1.3/Page.qml \
                 1.3/PageStack.qml \
                 1.3/pageUtils.js \
                 1.3/Panel.qml \
                 1.3/ProgressBar.qml \
                 1.3/PullToRefresh.qml \
                 1.3/Scrollbar.qml \
                 1.3/Sections.qml \
                 1.3/Slider.qml \
                 1.3/Switch.qml \
                 1.3/TabBar.qml \
                 1.3/Tab.qml \
                 1.3/Tabs.qml \
                 1.3/TextArea.qml \
                 1.3/TextCursor.qml \
                 1.3/TextField.qml \
                 1.3/TextInputPopover.qml \
                 1.3/Toolbar.qml \
                 1.3/ToolbarButton.qml \
                 1.3/ToolbarItems.qml \
                 1.3/LomiriColors.qml \
                 1.3/LomiriListView.qml \
                 1.3/LomiriNumberAnimation.qml \
                 1.3/ListItemPopover.qml \
                 1.3/PageColumn.qml \
                 1.3/PageColumnsLayout.qml \
                 1.3/ProgressionSlot.qml \
                 1.3/ScrollView.qml \
                 1.3/PageHeader.qml \
                 1.3/Icon.qml

    DYNAMIC_QMLDIR = $$cat(qmldir-qt5, lines)
} else {
    QML_FILES += 2.0/ActionBar.qml \
                 2.0/ActivityIndicator.qml \
                 2.0/AdaptivePageLayout.qml \
                 2.0/AnimatedItem.qml \
                 2.0/AppHeader.qml \
                 2.0/Button.qml \
                 2.0/Captions.qml \
                 2.0/CheckBox.qml \
                 2.0/ComboButton.qml \
                 2.0/CrossFadeImage.qml \
                 2.0/dateUtils.js \
                 2.0/DraggingArea.qml \
                 2.0/InputHandler.qml \
                 2.0/MainView.qml \
                 2.0/OptionSelectorDelegate.qml \
                 2.0/OptionSelector.qml \
                 2.0/OrientationHelper.qml \
                 2.0/PageHeadConfiguration.qml \
                 2.0/PageHeadSections.qml \
                 2.0/PageHeadState.qml \
                 2.0/Page.qml \
                 2.0/PageStack.qml \
                 2.0/pageUtils.js \
                 2.0/Panel.qml \
                 2.0/ProgressBar.qml \
                 2.0/PullToRefresh.qml \
                 2.0/Scrollbar.qml \
                 2.0/Sections.qml \
                 2.0/Slider.qml \
                 2.0/Switch.qml \
                 2.0/TabBar.qml \
                 2.0/Tab.qml \
                 2.0/Tabs.qml \
                 2.0/TextArea.qml \
                 2.0/TextCursor.qml \
                 2.0/TextField.qml \
                 2.0/TextInputPopover.qml \
                 2.0/Toolbar.qml \
                 2.0/ToolbarButton.qml \
                 2.0/ToolbarItems.qml \
                 2.0/LomiriColors.qml \
                 2.0/LomiriListView.qml \
                 2.0/LomiriNumberAnimation.qml \
                 2.0/ListItemPopover.qml \
                 2.0/PageColumn.qml \
                 2.0/PageColumnsLayout.qml \
                 2.0/ProgressionSlot.qml \
                 2.0/ScrollView.qml \
                 2.0/PageHeader.qml \
                 2.0/Icon.qml

    DYNAMIC_QMLDIR = $$cat(qmldir-qt6, lines)
}

OTHER_FILES+= qmldir-qt5 \
             qmldir-qt6 \
             1.3/CrossFadeImage.qdoc \
             1.3/PageHeadConfiguration.qdoc \
             1.3/MainView.qdoc \
             1.3/Icon.qdoc

QT_FRAG_SHADERS = \
    1.0/shaders/colorizeimage.frag \
    1.2/shaders/colourshader.frag \
    Themes/Ambiance/shaders/magnifier.frag \
    Themes/Ambiance/shaders/partialcolorize.frag

QT_VERT_SHADERS = \
    Themes/Ambiance/shaders/magnifier.vert

# todo fixme
QSB_FILES_RESOURCES_PREFIX = '/uc'

OTHER_FILES += $$QT_FRAG_SHADERS $$QT_VERT_SHADERS

load(qsb)
load(lomiri_qml_plugin)
