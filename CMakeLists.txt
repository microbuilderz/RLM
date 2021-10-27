cmake_minimum_required(VERSION 3.6)
set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_VERBOSE_MAKEFILE ON)

set(INCPATH /usr/include)

function(printfiles arg)
    foreach(loop_var ${arg})
        MESSAGE(STATUS ${loop_var})
    endforeach(loop_var)
endfunction(printfiles)

project(RLM)

add_definitions(-DTERMINALLOG)
add_definitions(-DENGINEHOSTNAME="localhost")
add_definitions(-DFIXLOGINBOUND=1)
add_definitions(-DFIXLOGOUTBOUND=1)
add_definitions(-DRLMPORT=4500)
add_definitions(-DDEFINECOLORS)
#add_definitions(-DFIXENGINELOG="./MsgLog")

add_subdirectory("SocketServer")
add_subdirectory("SocketClient")

add_subdirectory("FixEngine")
add_subdirectory("PluginManager")

add_subdirectory("EngineManager")
add_subdirectory("TestStaticPlugin")
add_subdirectory("DynamicEnginePlugin")

add_subdirectory("Doc")

#message(${CMAKE_CXX_FLAGS_RELEASE})