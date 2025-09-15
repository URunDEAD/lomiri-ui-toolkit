function(lomiri_target_prepare_headers target)
    set(options)
    set(oneValueArgs SUBDIRECTORY)
    set(multiValueArgs)
    cmake_parse_arguments(PARSE_ARGV 1 LTPH "${options}" "${oneValueArgs}" "${multiValueArgs}")

    get_target_property(sourceFiles ${target} SOURCES)
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${LTPH_SUBDIRECTORY}/private")
    target_include_directories(${target} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    )
    set(lomiriPublicHeaders)
    set(lomiriPrivateHeaders)
    foreach(file ${sourceFiles})
        if(${file} MATCHES "_p\.h$")
            file(COPY "${file}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${LTPH_SUBDIRECTORY}/private")
            list(APPEND lomiriPrivateHeaders "${file}")
        elseif(${file} MATCHES "\.h$")
            file(COPY "${file}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${LTPH_SUBDIRECTORY}")
            list(APPEND lomiriPublicHeaders "${file}")
        endif()
    endforeach()
    set_property(TARGET ${target} PROPERTY
        LOMIRI_PUBLIC_HEADERS ${lomiriPublicHeaders}
    )
    set_property(TARGET ${target} PROPERTY
        LOMIRI_PRIVATE_HEADERS ${lomiriPrivateHeaders}
    )
endfunction()

function(lomiri_target_install_headers target)
    set(options)
    set(oneValueArgs DESTINATION)
    set(multiValueArgs EXTRA_FILES)
    cmake_parse_arguments(PARSE_ARGV 1 LTIH "${options}" "${oneValueArgs}" "${multiValueArgs}")

    get_target_property(lomiriPrivateHeaders ${target} LOMIRI_PRIVATE_HEADERS)
    set(extraPrivateHeaders ${LTIH_EXTRA_FILES})
    list(FILTER extraPrivateHeaders INCLUDE REGEX "_p\.h$")
    list(APPEND lomiriPrivateHeaders ${extraPrivateHeaders})
    list(REMOVE_DUPLICATES lomiriPrivateHeaders)

    get_target_property(lomiriPublicHeaders ${target} LOMIRI_PUBLIC_HEADERS)
    set(extraPublicHeaders ${LTIH_EXTRA_FILES})
    list(FILTER extraPublicHeaders EXCLUDE REGEX "_p\.h$")
    list(APPEND lomiriPublicHeaders ${extraPublicHeaders})
    list(REMOVE_DUPLICATES lomiriPublicHeaders)

    install(FILES ${lomiriPublicHeaders}
        DESTINATION "${LTIH_DESTINATION}"
        COMPONENT Devel
    )
    install(FILES ${lomiriPrivateHeaders}
        DESTINATION "${LTIH_DESTINATION}/private"
        COMPONENT Devel
    )
endfunction()
