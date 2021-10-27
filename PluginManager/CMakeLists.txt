cmake_minimum_required(VERSION 3.6)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

function(PluginManager)
    MESSAGE(STATUS "--------------------PluginManager CMake----------------------")
    project(PluginManager)


    FIND_PACKAGE(APR)
    FIND_PACKAGE(dl)

    set(INC_DIR                     ${PROJECT_SOURCE_DIR}
                                    ${PROJECT_SOURCE_DIR}/inc/
                                    ${RLM_SOURCE_DIR}/TestStaticPlugin/inc/
                                    ${APR_INCLUDES})


    MESSAGE(STATUS "included dirs :")
    printfiles("${INC_DIR}")
    include_directories(${INC_DIR})


    set(LIBS                        ${LIBS}
                                    ${APR_LIBRARIES}
                                    ${dl_LIBRARIES})

    MESSAGE(STATUS "included libs :")
    printfiles("${LIBS}")


    set(SOURCE_FILES                src/utils/Path.cpp
                                    src/utils/Directory.cpp
                                    src/utils/base.cpp
                                    src/pluginFramework/PluginManager.cpp
                                    src/pluginFramework/DynamicLibrary.cpp)

    add_library(PluginManager_OBJECT OBJECT ${SOURCE_FILES} )

    add_executable(PluginManager    $<TARGET_OBJECTS:PluginManager_OBJECT>
                                    src/MasterRLM.cpp
                                    src/EngineManager.cpp)

    add_dependencies(PluginManager TestStaticPlugin)
    target_link_libraries(PluginManager TestStaticPlugin ${LIBS})
    set_target_properties(PluginManager PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${RLM_SOURCE_DIR}/bin)
    install(TARGETS PluginManager DESTINATION ${RLM_SOURCE_DIR}/bin)
endfunction(PluginManager)

PluginManager()