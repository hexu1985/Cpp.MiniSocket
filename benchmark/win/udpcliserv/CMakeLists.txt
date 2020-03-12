set(UNP_LIB unp-static)

add_executable(udpcli udpcli.cpp dg_cli.cpp)
target_link_libraries(udpcli ${UNP_LIB} ${LIBS_SYSTEM})

add_executable(udpserv udpserv.cpp dg_echo.cpp)
target_link_libraries(udpserv ${UNP_LIB} ${LIBS_SYSTEM})

add_executable(udpserv_byname udpserv_byname.cpp dg_echo.cpp)
target_link_libraries(udpserv_byname ${UNP_LIB} ${LIBS_SYSTEM})

add_executable(udpcli_byname udpcli_byname.cpp dg_cli.cpp)
target_link_libraries(udpcli_byname ${UNP_LIB} ${LIBS_SYSTEM})

