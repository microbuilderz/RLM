cmake_minimum_required(VERSION 3.6)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

function(TestStaticPlugin)
    MESSAGE(STATUS "--------------------TestStaticPlugin CMake----------------------")
    project(TestStaticPlugin)

    set(INC_DIR                             ${PROJECT_SOURCE_DIR}
                                            ${PROJECT_SOURCE_DIR}/inc
                                            ${RLM_SOURCE_DIR}/PluginManager/inc/)
    MESSAGE(STATUS "included dirs :")
    printfiles("${INC_DIR}")
    include_directories(${INC_DIR})


    set(LIBS ${LIBS})
    MESSAGE(STATUS "included libs :")
    printfiles("${LIBS}")


    set(SOURCE_FILES                        src/RLMEnginePluggable.cpp
                                            src/static_plugin.cpp)


    add_library(TestStaticPlugin STATIC ${SOURCE_FILES})
    target_link_libraries(TestStaticPlugin  ${LIBS})
    set_target_properties(TestStaticPlugin PROPERTIES ARCHIVE_OUTPUT_DIRECTORY  ${RLM_SOURCE_DIR}/bin/plugin)
    install(TARGETS TestStaticPlugin DESTINATION ${RLM_SOURCE_DIR}/bin/plugin)
endfunction(TestStaticPlugin)

TestStaticPlugin()