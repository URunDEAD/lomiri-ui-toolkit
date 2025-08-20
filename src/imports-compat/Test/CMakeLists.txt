set(TARGETPATH "Ubuntu/Test")
set(IMPORT_VERSION "0.1")

# Define QML files
set(QML_FILES
    MouseTouchAdaptor.qml
    TestExtras.qml
)

# Install QML files
install(FILES ${QML_FILES}
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/qml/${TARGETPATH}
)
