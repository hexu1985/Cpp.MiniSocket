set(UNP_LIB unp-static)
aux_source_directory(. SRC_FILE_LIST)

foreach(SRC_FILE ${SRC_FILE_LIST})
    get_filename_component(EXE_FILE ${SRC_FILE} NAME_WE)
    add_executable(${EXE_FILE} ${SRC_FILE})
    target_link_libraries(${EXE_FILE} ${UNP_LIB} ${LIBS_SYSTEM})
endforeach()

