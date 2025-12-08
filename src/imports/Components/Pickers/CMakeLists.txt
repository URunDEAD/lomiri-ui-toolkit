set_source_files_properties(2.0/PickerPanel.qml
    PROPERTIES
        QT_QML_SINGLETON_TYPE TRUE
)

qt_add_qml_module(LomiriComponentsPickers
    URI Lomiri.Components.Pickers
    VERSION 2.0
    PLUGIN_TARGET LomiriComponentsPickers
    OUTPUT_DIRECTORY "${QML_BUILD_DIR}/Lomiri/Components/Pickers"
    NO_GENERATE_EXTRA_QMLDIRS
    NO_PLUGIN_OPTIONAL
    QML_FILES
        2.0/DatePicker.qml
        2.0/DayModel.qml
        2.0/Dialer.qml
        2.0/DialerHand.qml
        2.0/DialerHandGroup.qml
        2.0/HoursModel.qml
        2.0/MinutesModel.qml
        2.0/MonthModel.qml
        2.0/Picker.qml
        2.0/PickerDelegate.qml
        2.0/PickerModelBase.qml
        2.0/PickerPanel.qml
        2.0/PickerRow.qml
        2.0/SecondsModel.qml
        2.0/YearModel.qml
)

install_qml_module(LomiriComponentsPickers)
