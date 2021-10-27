cmake_minimum_required(VERSION 3.6)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

function(SocketClientAPR)
    MESSAGE(STATUS "--------------------SocketClientAPR CMake----------------------")
    project(SocketClientAPR)

    FIND_PACKAGE(APR)

    set(INC_DIR ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/inc/ ${APR_INCLUDES} )

    MESSAGE(STATUS "included dirs :")
    printfiles("${INC_DIR}")

    include_directories(${INC_DIR})
    set(LIBS ${LIBS} ${APR_LIBRARIES})

    MESSAGE(STATUS "included libs :")
    printfiles("${LIBS}")

    set(SOURCE_FILES    src/Main.cpp
            src/ClientHandler.cpp)

    add_executable(SocketClientAPR ${SOURCE_FILES})
    target_link_libraries(SocketClientAPR ${LIBS})
    target_compile_definitions(SocketClientAPR PRIVATE CLIENTAPRMODE)
    set_target_properties(SocketClientAPR PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${RLM_SOURCE_DIR}/bin)
    install(TARGETS SocketClientAPR DESTINATION ${RLM_SOURCE_DIR}/bin)
endfunction(SocketClientAPR)

function(SocketClientSSL)
    MESSAGE(STATUS "--------------------SocketClientSSL CMake----------------------")
    project(SocketClientSSL)

    FIND_PACKAGE(APR)
    FIND_PACKAGE(SSL)
    FIND_PACKAGE(Crypto)

    set(INC_DIR ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/inc/ ${APR_INCLUDES} ${SSL_INCLUDES} )

    MESSAGE(STATUS "included dirs :")
    printfiles("${INC_DIR}")

    include_directories(${INC_DIR})
    set(LIBS ${LIBS} ${SSL_LIBRARIES} ${APR_LIBRARIES} ${Crypto_LIBRARIES})

    MESSAGE(STATUS "included libs :")
    printfiles("${LIBS}")

    set(SOURCE_FILES    src/Main.cpp
                        src/ClientHandler.cpp)

    add_executable(SocketClientSSL ${SOURCE_FILES})
    target_link_libraries(SocketClientSSL ${LIBS})
    target_compile_definitions(SocketClientSSL PRIVATE CLIENTSSLMODE)
    set_target_properties(SocketClientSSL PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${RLM_SOURCE_DIR}/bin)
    install(TARGETS SocketClientSSL DESTINATION ${RLM_SOURCE_DIR}/bin)
endfunction(SocketClientSSL)

SocketClientAPR()
SocketClientSSL()