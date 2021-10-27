cmake_minimum_required(VERSION 3.6)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

function(EngineManager)
    MESSAGE(STATUS "--------------------EngineManager CMake----------------------")
    project(EngineManager)

    FIND_PACKAGE(OnixS)
    FIND_PACKAGE(APR)
    FIND_PACKAGE(SSL)
    FIND_PACKAGE(pthread)
    FIND_PACKAGE(Crypto)
    FIND_PACKAGE(dl)

    set(INC_DIR                         ${PROJECT_SOURCE_DIR}
                                        ${PROJECT_SOURCE_DIR}/inc/
                                        ${RLM_SOURCE_DIR}/FixEngine/inc/
                                        ${RLM_SOURCE_DIR}/PluginManager/inc/
                                        ${RLM_SOURCE_DIR}/SocketServer/inc/
                                        ${APR_INCLUDES}
                                        ${SSL_INCLUDES}
                                        ${pthread_INCLUDES})
    MESSAGE(STATUS "included dirs :")
    printfiles("${INC_DIR}")
    include_directories(${INC_DIR})

    set(LIBS                            ${LIBS}
                                        ${APR_LIBRARIES}
                                        ${SSL_LIBRARIES}
                                        ${Crypto_LIBRARIES}
                                        ${pthread_LIBRARIES}
                                        ${OnixS_LIBRARIES}
                                        ${dl_LIBRARIES})
    MESSAGE(STATUS "included libs :")
    printfiles("${LIBS}")

    set(SOURCE_FILES                    src/Main.cpp
                                        src/PEngineManager.cpp)
    add_executable(EngineManager        ${SOURCE_FILES}
                                        $<TARGET_OBJECTS:MultiplexSSL_OBJECT>
                                        $<TARGET_OBJECTS:PluginManager_OBJECT>
                                        $<TARGET_OBJECTS:FixEngine_OBJECT>)

    target_link_libraries(EngineManager ${LIBS}  )
    set_target_properties(EngineManager PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${RLM_SOURCE_DIR}/bin)
    install(TARGETS EngineManager DESTINATION ${RLM_SOURCE_DIR}/bin)
endfunction(EngineManager)

EngineManager()