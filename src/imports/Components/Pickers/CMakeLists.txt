set(QML_PLUGIN_DIR "${QML_PLUGIN_BASE_DIR}/Components/Pickers")

set(QML_FILES
    "2.0/DatePicker.qml"
    "2.0/DayModel.qml"
    "2.0/DialerHandGroup.qml"
    "2.0/DialerHand.qml"
    "2.0/Dialer.qml"
    "2.0/HoursModel.qml"
    "2.0/MinutesModel.qml"
    "2.0/MonthModel.qml"
    "2.0/PickerDelegate.qml"
    "2.0/PickerModelBase.qml"
    "2.0/PickerPanel.qml"
    "2.0/Picker.qml"
    "2.0/PickerRow.qml"
    "2.0/SecondsModel.qml"
    "2.0/YearModel.qml"
)

install(FILES ${QML_FILES} DESTINATION ${QML_PLUGIN_DIR}/2.0)
install(FILES "qmldir-qt6" DESTINATION ${QML_PLUGIN_DIR}/ RENAME "qmldir")
