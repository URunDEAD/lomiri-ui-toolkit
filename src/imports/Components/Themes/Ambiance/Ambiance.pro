TARGETPATH = Lomiri/Components/Themes/Ambiance

ARTWORK_FILES += artwork/background_paper@27.png \
             artwork/bubble_arrow@20.png \
             artwork/bubble_arrow@30.png \
             artwork/bubble_arrow@8.png \
             artwork/bubble_shadow@20.png \
             artwork/bubble_shadow@20.sci \
             artwork/bubble_shadow@30.png \
             artwork/bubble_shadow@30.sci \
             artwork/bubble_shadow@8.png \
             artwork/bubble_shadow@8.sci \
             artwork/caret_noshadow@27.png \
             artwork/chevron@27.png \
             artwork/chevron_down@30.png \
             artwork/cross@30.png \
             artwork/header_overflow_dropshadow@27.png \
             artwork/header_overflow_dropshadow@27.sci \
             artwork/PageHeaderBaseDividerBottom@18.png \
             artwork/PageHeaderBaseDividerLight@18.png \
             artwork/PageHeaderBaseDividerLight@18.sci \
             artwork/progression_divider@18.png \
             artwork/ScrollbarBottomIdle@8.png \
             artwork/ScrollbarBottomPressed@8.png \
             artwork/ScrollbarLeftIdle@8.png \
             artwork/ScrollbarLeftPressed@8.png \
             artwork/ScrollbarRightIdle@8.png \
             artwork/ScrollbarRightPressed@8.png \
             artwork/ScrollbarTopIdle@8.png \
             artwork/ScrollbarTopPressed@8.png \
             artwork/spinner@16.png \
             artwork/spinner@32.png \
             artwork/spinner@8.png \
             artwork/stroke_button@27.png \
             artwork/tick@30.png \
             artwork/toolbar_dropshadow@20.png \
             artwork/toolkit_scrollbar-stepper.svg

equals(QT_MAJOR_VERSION, 5) {
QML_FILES += 1.2/ActivityIndicatorStyle.qml \
             1.2/BubbleShape.qml \
             1.2/ButtonForeground.qml \
             1.2/ButtonStyle.qml \
             1.2/CheckBoxStyle.qml \
             1.2/ComboButtonStyle.qml \
             1.2/DatePickerStyle.qml \
             1.2/DialerHandStyle.qml \
             1.2/DialerStyle.qml \
             1.2/DialogForegroundStyle.qml \
             1.2/HeadDividerStyle.qml \
             1.2/HeaderStyle.qml \
             1.2/HighlightMagnifier.qml \
             1.2/ListItemOptionSelectorStyle.qml \
             1.2/ListItemStyle.qml \
             1.2/MainViewStyle.qml \
             1.2/OptionSelectorStyle.qml \
             1.2/OverflowPanel.qml \
             1.2/PageHeadButton.qml \
             1.2/PageHeadStyle.qml \
             1.2/Palette.qml \
             1.2/PartialColorize.qml \
             1.2/PickerDelegateStyle.qml \
             1.2/PickerStyle.qml \
             1.2/PopoverForegroundStyle.qml \
             1.2/ProgressBarStyle.qml \
             1.2/ProgressionVisualStyle.qml \
             1.2/PullToRefreshStyle.qml \
             1.2/ScrollbarStyle.qml \
             1.2/SheetForegroundStyle.qml \
             1.2/SliderStyle.qml \
             1.2/SwitchStyle.qml \
             1.2/TabBarStyle.qml \
             1.2/TextAreaStyle.qml \
             1.2/TextCursorStyle.qml \
             1.2/TextFieldStyle.qml \
             1.2/ToolbarButtonStyle.qml \
             1.2/DeprecatedToolbarStyle.qml \
             1.3/AmbianceNormal.qml \
             1.3/AmbianceSelected.qml \
             1.3/ActionBarStyle.qml \
             1.3/ActivityIndicatorStyle.qml \
             1.3/BubbleShape.qml \
             1.3/ButtonForeground.qml \
             1.3/ButtonStyle.qml \
             1.3/CheckBoxStyle.qml \
             1.3/ComboButtonStyle.qml \
             1.3/DatePickerStyle.qml \
             1.3/DialerHandStyle.qml \
             1.3/DialerStyle.qml \
             1.3/DialogForegroundStyle.qml \
             1.3/HighlightMagnifier.qml \
             1.3/ListItemOptionSelectorStyle.qml \
             1.3/ListItemStyle.qml \
             1.3/MainViewStyle.qml \
             1.3/OptionSelectorStyle.qml \
             1.3/OverflowPanel.qml \
             1.3/PageHeadButton.qml \
             1.3/PageHeadStyle.qml \
             1.3/Palette.qml \
             1.3/PartialColorize.qml \
             1.3/PickerDelegateStyle.qml \
             1.3/PickerStyle.qml \
             1.3/PopoverForegroundStyle.qml \
             1.3/ProgressBarStyle.qml \
             1.3/ProgressionVisualStyle.qml \
             1.3/PullToRefreshStyle.qml \
             1.3/ScrollbarStyle.qml \
             1.3/ScrollingActionBarStyle.qml \
             1.3/SectionsStyle.qml \
             1.3/SheetForegroundStyle.qml \
             1.3/SliderStyle.qml \
             1.3/SwitchStyle.qml \
             1.3/TabBarStyle.qml \
             1.3/TextAreaStyle.qml \
             1.3/TextCursorStyle.qml \
             1.3/TextFieldStyle.qml \
             1.3/ToolbarButtonStyle.qml \
             1.3/ToolbarStyle.qml \
             1.3/IconButtonStyle.qml \
             1.3/PageHeaderStyle.qml \
             1.3/BottomEdgeHintStyle.qml \
             1.3/BottomEdgeStyle.qml \
             1.3/FocusShape.qml \
             1.3/SectionsForPageHead.qml \
             1.3/SectionsForPageHeadStyle.qml \
             $$ARTWORK_FILES

    DYNAMIC_QMLDIR = $$cat(qmldir-qt5, lines)
} else {

QML_FILES += 2.0/AmbianceNormal.qml \
             2.0/AmbianceSelected.qml \
             2.0/ActionBarStyle.qml \
             2.0/ActivityIndicatorStyle.qml \
             2.0/BubbleShape.qml \
             2.0/ButtonForeground.qml \
             2.0/ButtonStyle.qml \
             2.0/CheckBoxStyle.qml \
             2.0/ComboButtonStyle.qml \
             2.0/DatePickerStyle.qml \
             2.0/DialerHandStyle.qml \
             2.0/DialerStyle.qml \
             2.0/DialogForegroundStyle.qml \
             2.0/HighlightMagnifier.qml \
             2.0/ListItemOptionSelectorStyle.qml \
             2.0/ListItemStyle.qml \
             2.0/MainViewStyle.qml \
             2.0/OptionSelectorStyle.qml \
             2.0/OverflowPanel.qml \
             2.0/PageHeadButton.qml \
             2.0/PageHeadStyle.qml \
             2.0/Palette.qml \
             2.0/PartialColorize.qml \
             2.0/PickerDelegateStyle.qml \
             2.0/PickerStyle.qml \
             2.0/PopoverForegroundStyle.qml \
             2.0/ProgressBarStyle.qml \
             2.0/ProgressionVisualStyle.qml \
             2.0/PullToRefreshStyle.qml \
             2.0/ScrollbarStyle.qml \
             2.0/ScrollingActionBarStyle.qml \
             2.0/SectionsStyle.qml \
             2.0/SheetForegroundStyle.qml \
             2.0/SliderStyle.qml \
             2.0/SwitchStyle.qml \
             2.0/TabBarStyle.qml \
             2.0/TextAreaStyle.qml \
             2.0/TextCursorStyle.qml \
             2.0/TextFieldStyle.qml \
             2.0/ToolbarButtonStyle.qml \
             2.0/ToolbarStyle.qml \
             2.0/IconButtonStyle.qml \
             2.0/PageHeaderStyle.qml \
             2.0/BottomEdgeHintStyle.qml \
             2.0/BottomEdgeStyle.qml \
             2.0/FocusShape.qml \
             2.0/SectionsForPageHead.qml \
             2.0/SectionsForPageHeadStyle.qml \
             $$ARTWORK_FILES

    DYNAMIC_QMLDIR = $$cat(qmldir-qt6, lines)
}

load(lomiri_qml_module)

OTHER_FILES+=qmldir
