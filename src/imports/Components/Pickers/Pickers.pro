TARGETPATH = Lomiri/Components/Pickers

equals(QT_MAJOR_VERSION, 5) {
    QML_FILES += 1.2/DatePicker.qml \
                 1.2/DayModel.qml \
                 1.2/DialerHandGroup.qml \
                 1.2/DialerHand.qml \
                 1.2/Dialer.qml \
                 1.2/HoursModel.qml \
                 1.2/MinutesModel.qml \
                 1.2/MonthModel.qml \
                 1.2/PickerDelegate.qml \
                 1.2/PickerModelBase.qml \
                 1.2/PickerPanel.qml \
                 1.2/Picker.qml \
                 1.2/PickerRow.qml \
                 1.2/SecondsModel.qml \
                 1.2/YearModel.qml \
                 1.3/DatePicker.qml \
                 1.3/DayModel.qml \
                 1.3/DialerHandGroup.qml \
                 1.3/DialerHand.qml \
                 1.3/Dialer.qml \
                 1.3/HoursModel.qml \
                 1.3/MinutesModel.qml \
                 1.3/MonthModel.qml \
                 1.3/PickerDelegate.qml \
                 1.3/PickerModelBase.qml \
                 1.3/PickerPanel.qml \
                 1.3/Picker.qml \
                 1.3/PickerRow.qml \
                 1.3/SecondsModel.qml \
                 1.3/YearModel.qml
    DYNAMIC_QMLDIR = $$cat(qmldir-qt5, lines)
} else {
    QML_FILES += 2.0/DatePicker.qml \
                 2.0/DayModel.qml \
                 2.0/DialerHandGroup.qml \
                 2.0/DialerHand.qml \
                 2.0/Dialer.qml \
                 2.0/HoursModel.qml \
                 2.0/MinutesModel.qml \
                 2.0/MonthModel.qml \
                 2.0/PickerDelegate.qml \
                 2.0/PickerModelBase.qml \
                 2.0/PickerPanel.qml \
                 2.0/Picker.qml \
                 2.0/PickerRow.qml \
                 2.0/SecondsModel.qml \
                 2.0/YearModel.qml
    DYNAMIC_QMLDIR = $$cat(qmldir-qt6, lines)
}

OTHER_FILES+= qmldir-qt5 \
             qmldir-qt6

load(lomiri_qml_module)
