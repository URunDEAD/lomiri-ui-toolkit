set(QML_PLUGIN_DIR "${QML_PLUGIN_BASE_DIR}/Components/Pickers")

set(QML_FILES_1_2
    "1.2/DatePicker.qml"
    "1.2/DayModel.qml"
    "1.2/DialerHandGroup.qml"
    "1.2/DialerHand.qml"
    "1.2/Dialer.qml"
    "1.2/HoursModel.qml"
    "1.2/MinutesModel.qml"
    "1.2/MonthModel.qml"
    "1.2/PickerDelegate.qml"
    "1.2/PickerModelBase.qml"
    "1.2/PickerPanel.qml"
    "1.2/Picker.qml"
    "1.2/PickerRow.qml"
    "1.2/SecondsModel.qml"
    "1.2/YearModel.qml"
)

set(QML_FILES_1_3
    "1.3/DatePicker.qml"
    "1.3/DayModel.qml"
    "1.3/DialerHandGroup.qml"
    "1.3/DialerHand.qml"
    "1.3/Dialer.qml"
    "1.3/HoursModel.qml"
    "1.3/MinutesModel.qml"
    "1.3/MonthModel.qml"
    "1.3/PickerDelegate.qml"
    "1.3/PickerModelBase.qml"
    "1.3/PickerPanel.qml"
    "1.3/Picker.qml"
    "1.3/PickerRow.qml"
    "1.3/SecondsModel.qml"
    "1.3/YearModel.qml"
)

set(OTHER_FILES
    "qmldir"
)

set(ALL_QML_FILES
    ${QML_FILES_1_2}
    ${QML_FILES_1_3}
    ${OTHER_FILES}
)

install(FILES ${QML_FILES_1_2}
    DESTINATION ${QML_PLUGIN_DIR}/1.2
)

install(FILES ${QML_FILES_1_3}
    DESTINATION ${QML_PLUGIN_DIR}/1.3
)
