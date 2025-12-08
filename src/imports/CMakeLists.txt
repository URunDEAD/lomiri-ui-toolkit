# Reference: https://doc.qt.io/qt-6/qt-query-qml-module.html
# Installs the specified QML component to ${QML_INSTALL_DIR}.
function(install_qml_module target)
    qt_query_qml_module(${target}
        URI module_uri
        VERSION module_version
        PLUGIN_TARGET module_plugin_target
        TARGET_PATH module_target_path
        QMLDIR module_qmldir
        TYPEINFO module_typeinfo
        QML_FILES module_qml_files
        QML_FILES_DEPLOY_PATHS qml_files_deploy_paths
        RESOURCES module_resources
        RESOURCES_DEPLOY_PATHS resources_deploy_paths
    )

    set(install_dir "${QML_INSTALL_DIR}/${module_target_path}")

    # Install the plugin and the generated qmldir/typeinfo files
    install(TARGETS ${module_plugin_target} DESTINATION "${install_dir}")
    install(FILES "${module_qmldir}" DESTINATION "${install_dir}")
    if(DEFINED module_typeinfo)
        install(FILES "${module_typeinfo}" DESTINATION "${install_dir}")
    endif()

    # Install QML files
    list(LENGTH module_qml_files num_files)
    if(${num_files} GREATER 0)
        math(EXPR last_index "${num_files} - 1")
        foreach(i RANGE 0 ${last_index})
            list(GET module_qml_files       ${i} src_file)
            list(GET qml_files_deploy_paths ${i} deploy_path)
            get_filename_component(dst_name "${deploy_path}" NAME)
            get_filename_component(dest_dir "${deploy_path}" DIRECTORY)
            install(FILES "${src_file}" DESTINATION "${install_dir}/${dest_dir}" RENAME "${dst_name}")
        endforeach()
    endif()

    # Install resources
    list(LENGTH module_resources num_files)
    if(${num_files} GREATER 0)
        math(EXPR last_index "${num_files} - 1")
        foreach(i RANGE 0 ${last_index})
            list(GET module_resources       ${i} src_file)
            list(GET resources_deploy_paths ${i} deploy_path)
            get_filename_component(dst_name "${deploy_path}" NAME)
            get_filename_component(dest_dir "${deploy_path}" DIRECTORY)
            install(FILES "${src_file}" DESTINATION "${install_dir}/${dest_dir}" RENAME "${dst_name}")
        endforeach()
    endif()
endfunction()

add_subdirectory(Components)
add_subdirectory(Layouts)
add_subdirectory(Metrics)
add_subdirectory(PerformanceMetrics)
add_subdirectory(Test)
add_subdirectory(SystemInfo)
