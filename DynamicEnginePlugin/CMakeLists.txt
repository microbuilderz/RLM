cmake_minimum_required(VERSION 3.6)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-z,defs")

function(DynamicEnginePlugin varName)
    MESSAGE(STATUS "--------------------DynamicEnginePlugin_${varName} CMake----------------------")
    project(DynamicEnginePlugin_${varName})

    FIND_PACKAGE(OnixS)
    FIND_PACKAGE(pthread)

    add_definitions(-DDEFINECOLORS -D${varName})
    set(INC_DIR                             ${PROJECT_SOURCE_DIR}
                                            ${PROJECT_SOURCE_DIR}/inc
                                            ${RLM_SOURCE_DIR}/EngineManager/inc/
                                            ${RLM_SOURCE_DIR}/PluginManager/inc/
                                            ${RLM_SOURCE_DIR}/FixEngine/inc/)
    MESSAGE(STATUS "included dirs :")
    printfiles("${INC_DIR}")
    include_directories(${INC_DIR})


    set(LIBS                                ${LIBS}
                                            ${OnixS_LIBRARIES}
                                            ${pthread_LIBRARIES})
    MESSAGE(STATUS "included libs :")
    printfiles("${LIBS}")


    set(SOURCE_FILES                        src/RLMEngineDynamic.cpp
                                            src/cpp_plugin.cpp
                                            src/Timer.cpp)


    add_library(DynamicEnginePlugin_${varName} SHARED   ${SOURCE_FILES}
                                                        $<TARGET_OBJECTS:FixEngine_MassOrder_OBJECT>)

    set_property(TARGET DynamicEnginePlugin_${varName} PROPERTY POSITION_INDEPENDENT_CODE TRUE)
    target_compile_definitions(DynamicEnginePlugin_${varName} PRIVATE -DDEFINECOLORS)
    target_link_libraries(DynamicEnginePlugin_${varName}  ${LIBS})
    set_target_properties(DynamicEnginePlugin_${varName}   PROPERTIES
                                                LIBRARY_OUTPUT_DIRECTORY ${RLM_SOURCE_DIR}/bin/plugin
                                                OUTPUT_NAME ${PROJECT_NAME})

    install(TARGETS DynamicEnginePlugin_${varName} DESTINATION ${RLM_SOURCE_DIR}/bin/plugin)
endfunction(DynamicEnginePlugin)

DynamicEnginePlugin(PT)
DynamicEnginePlugin(NT)
DynamicEnginePlugin(NORCLIENT1)