file(GLOB UNP_LIB_SRC_LIST *.cpp)

add_library(unp SHARED ${UNP_LIB_SRC_LIST})
target_link_libraries(unp ${LIBS_SYSTEM}) 

add_library(unp-static STATIC ${UNP_LIB_SRC_LIST})

