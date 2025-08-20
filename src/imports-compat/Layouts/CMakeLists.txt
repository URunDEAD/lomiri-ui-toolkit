set(TARGETPATH "Ubuntu/Layouts")
set(IMPORT_VERSION "0.1")

# Define QML files
set(QML_FILES
    Layouts.qml
    ConditionalLayout.qml
    ItemLayout.qml
)

# Install QML files
install(FILES ${QML_FILES} qmldir
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/qml/${TARGETPATH}
)