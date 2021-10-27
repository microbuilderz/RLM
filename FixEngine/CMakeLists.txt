cmake_minimum_required(VERSION 3.6)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

#remove_definitions(-DTERMINALLOG)

function(FixEngine)
    MESSAGE(STATUS "--------------------FixEngine CMake----------------------")
    project(FixEngine)

    FIND_PACKAGE(OnixS)
    FIND_PACKAGE(pthread)

    set(INC_DIR                     ${PROJECT_SOURCE_DIR}
                                    ${PROJECT_SOURCE_DIR}/inc/
                                    ${OnixS_INCLUDES}
                                    ${pthread_INCLUDE_DIR})
    MESSAGE(STATUS "included dirs :")
    printfiles("${INC_DIR}")
    include_directories(${INC_DIR})

    set(LIBS                        ${LIBS}
                                    ${OnixS_LIBRARIES}
                                    ${pthread_LIBRARIES})
    MESSAGE(STATUS "included libs :")
    printfiles("${LIBS}")

    set(SOURCE_FILES                src/engine/RLMEngineListener.cpp
                                    src/engine/RLMEngineManager.cpp
                                    src/engine/tests/MassOrderGenerator.cpp
                                    src/engine/tests/Timer.cpp
                                    src/engine/takers/TakersListener.cpp
                                    src/engine/makers/MakersListener.cpp
                                    src/engine/adapter/SimpleLPAdapter.cpp
                                    src/engine/adapter/SimpleClientAdapter.cpp)

    add_library(FixEngine_MassOrder_OBJECT OBJECT   src/engine/tests/MassOrderGenerator.cpp
                                                    src/engine/adapter/SimpleLPAdapter.cpp
                                                    src/engine/adapter/SimpleClientAdapter.cpp)

    set_property(TARGET FixEngine_MassOrder_OBJECT PROPERTY POSITION_INDEPENDENT_CODE TRUE)

    add_library(FixEngine_OBJECT OBJECT ${SOURCE_FILES} )

    add_executable(FixEngine        $<TARGET_OBJECTS:FixEngine_OBJECT>
                                    src/RLM.cpp)

    target_link_libraries(FixEngine ${LIBS})
    set_target_properties(FixEngine PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${RLM_SOURCE_DIR}/bin)
    install(TARGETS FixEngine DESTINATION ${RLM_SOURCE_DIR}/bin)
endfunction(FixEngine)

FixEngine()