cmake_minimum_required(VERSION 3.6)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

function(MultiplexSSL)
    MESSAGE(STATUS "--------------------MultiplexSSL CMake----------------------")
    project(MultiplexSSL)

    FIND_PACKAGE(APR)
    FIND_PACKAGE(SSL)
    FIND_PACKAGE(Crypto)

    set(INC_DIR                         ${PROJECT_SOURCE_DIR}
                                        ${PROJECT_SOURCE_DIR}/inc/
                                        ${APR_INCLUDES}
                                        ${SSL_INCLUDES})

    MESSAGE(STATUS "included dirs :")
    printfiles("${INC_DIR}")
    include_directories(${INC_DIR})
    set(LIBS                            ${LIBS}
                                        ${SSL_LIBRARIES}
                                        ${APR_LIBRARIES}
                                        ${Crypto_LIBRARIES})
    MESSAGE(STATUS "included libs :")
    printfiles("${LIBS}")

    set(SOURCE_FILES                    src/multiplex/BaseServer.cpp
                                        src/multiplex/SSLServer.cpp
                                        src/utils/SSLCertificate.cpp)
    add_library(MultiplexSSL_OBJECT OBJECT ${SOURCE_FILES} )

    add_executable(MultiplexSSL         $<TARGET_OBJECTS:MultiplexSSL_OBJECT>
                                        src/Main.cpp)

    target_link_libraries(MultiplexSSL ${LIBS})
    target_compile_definitions(MultiplexSSL PRIVATE MULTIPLEXSSL)
    set_target_properties(MultiplexSSL PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${RLM_SOURCE_DIR}/bin)
    install(TARGETS MultiplexSSL DESTINATION ${RLM_SOURCE_DIR}/bin)
endfunction(MultiplexSSL)

function(MultiplexAPR)
    MESSAGE(STATUS "--------------------MultiplexAPR CMake----------------------")
    project(MultiplexAPR)

    FIND_PACKAGE(APR)

    set(INC_DIR                         ${PROJECT_SOURCE_DIR}
                                        ${PROJECT_SOURCE_DIR}/inc/
                                        ${APR_INCLUDES} )
    MESSAGE(STATUS "included dirs :")
    printfiles("${INC_DIR}")
    include_directories(${INC_DIR})

    set(LIBS                            ${LIBS}
                                        ${APR_LIBRARIES})
    MESSAGE(STATUS "included libs :")
    printfiles("${LIBS}")

    set(SOURCE_FILES                    src/multiplex/BaseServer.cpp
                                        src/multiplex/NoSSLServer.cpp)
    add_library(MultiplexAPR_OBJECT OBJECT ${SOURCE_FILES} )

    add_executable(MultiplexAPR         $<TARGET_OBJECTS:MultiplexAPR_OBJECT>
                                        src/Main.cpp)

    target_link_libraries(MultiplexAPR ${LIBS})
    target_compile_definitions(MultiplexAPR PRIVATE MULTIPLEXAPR)
    set_target_properties(MultiplexAPR PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${RLM_SOURCE_DIR}/bin)
    install(TARGETS MultiplexAPR DESTINATION ${RLM_SOURCE_DIR}/bin)
endfunction(MultiplexAPR)

function(MultithreadAPR)
    MESSAGE(STATUS "--------------------MultithreadAPR CMake----------------------")
    project(MultithreadAPR)

    FIND_PACKAGE(APR)

    set(INC_DIR                         ${PROJECT_SOURCE_DIR}
                                        ${PROJECT_SOURCE_DIR}/inc/
                                        ${APR_INCLUDES})
    MESSAGE(STATUS "included dirs :")
    printfiles("${INC_DIR}")
    include_directories(${INC_DIR})

    set(LIBS                            ${LIBS}
                                        ${APR_LIBRARIES})
    MESSAGE(STATUS "included libs :")
    printfiles("${LIBS}")

    set(SOURCE_FILES                    src/multithread/BaseServer.cpp
                                        src/multithread/NoSSLServer.cpp)
    add_library(MultithreadAPR_OBJECT OBJECT ${SOURCE_FILES} )

    add_executable(MultithreadAPR       $<TARGET_OBJECTS:MultithreadAPR_OBJECT>
                                        src/Main.cpp)
    target_link_libraries(MultithreadAPR ${LIBS})
    target_compile_definitions(MultithreadAPR PRIVATE MULTITHREADAPR)
    set_target_properties(MultithreadAPR PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${RLM_SOURCE_DIR}/bin)
    install(TARGETS MultithreadAPR DESTINATION ${RLM_SOURCE_DIR}/bin)
endfunction(MultithreadAPR)

function(MultithreadSSL)
    MESSAGE(STATUS "--------------------MultithreadSSL CMake----------------------")
    project(MultithreadSSL)


    FIND_PACKAGE(APR)
    FIND_PACKAGE(SSL)
    FIND_PACKAGE(Crypto)


    set(INC_DIR                         ${PROJECT_SOURCE_DIR}
                                        ${PROJECT_SOURCE_DIR}/inc/
                                        ${APR_INCLUDES} ${SSL_INCLUDES})
    MESSAGE(STATUS "included dirs :")
    printfiles("${INC_DIR}")
    include_directories(${INC_DIR})


    set(LIBS                            ${LIBS}
                                        ${SSL_LIBRARIES}
                                        ${APR_LIBRARIES}
                                        ${Crypto_LIBRARIES})
    MESSAGE(STATUS "included libs :")
    printfiles("${LIBS}")


    set(SOURCE_FILES                    src/multithread/BaseServer.cpp
                                        src/multithread/SSLServer.cpp
                                        src/utils/SSLCertificate.cpp)
    add_library(MultithreadSSL_OBJECT OBJECT ${SOURCE_FILES} )


    add_executable(MultithreadSSL       $<TARGET_OBJECTS:MultithreadSSL_OBJECT>
                                        src/Main.cpp )

    target_link_libraries(MultithreadSSL ${LIBS})
    target_compile_definitions(MultithreadSSL PRIVATE MULTITHREADSSL)
    set_target_properties(MultithreadSSL PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${RLM_SOURCE_DIR}/bin)
    install(TARGETS MultithreadSSL DESTINATION ${RLM_SOURCE_DIR}/bin)
endfunction(MultithreadSSL)

MultithreadAPR()
MultithreadSSL()
MultiplexAPR()
MultiplexSSL()

MESSAGE(STATUS "-------------------------------------------------------------------")
printfiles("${MultiplexSSL_SOURCES}")
MESSAGE(STATUS "-------------------------------------------------------------------")