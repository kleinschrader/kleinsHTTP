cmake_minimum_required(VERSION 3.10)

# set the project name
PROJECT(kleinsHTTP VERSION 0.3.4)

option (FORCE_COLORED_OUTPUT "Always produce ANSI-colored output (GNU/Clang only)." FALSE)

if (${FORCE_COLORED_OUTPUT})
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
       add_compile_options (-fdiagnostics-color=always)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
       add_compile_options (-fcolor-diagnostics)
    endif ()
endif ()

SET(libsrc
./source/sslSocket/sslSocket.cpp
./source/httpParser/httpParser.cpp
./source/httpServer/httpServer.cpp
./source/socketBase/socketBase.cpp
./source/packet/packet.cpp
./source/tcpSocket/tcpSocket.cpp
./source/sessionBase/sessionBase.cpp
./source/tcpConnection/tcpConnection.cpp
./source/connectionBase/connectionBase.cpp
./source/sslConnection/sslConnection.cpp
./source/metricsServer/metricsServer.cpp
./source/metricBase/metricBase.cpp
./source/counterMetric/counterMetric.cpp
./source/histogramMetric/histogramMetric.cpp
./source/gaugeMetric/gaugeMetric.cpp)

SET(libhead
./source/socketBase/socketBase.h
./source/connectionBase/connectionBase.h
./source/sslSocket/sslSocket.h
./source/httpParser/httpParser.h
./source/httpServer/httpServer.h
./source/packet/packet.h
./source/tcpSocket/tcpSocket.h
./source/sessionBase/sessionBase.h
./source/tcpConnection/tcpConnection.h
./source/sslConnection/sslConnection.h
./source/metricsServer/metricsServer.h
./source/metricBase/metricBase.h
./source/counterMetric/counterMetric.h
./source/histogramMetric/histogramMetric.h
./source/gaugeMetric/gaugeMetric.h
./source/templates.h)

ADD_LIBRARY(kleinsHTTP-shared SHARED "${libsrc}")
ADD_LIBRARY(kleinsHTTP-static STATIC "${libsrc}")

set_property(TARGET kleinsHTTP-shared PROPERTY CXX_STANDARD 17)
set_property(TARGET kleinsHTTP-static PROPERTY CXX_STANDARD 17)

set_target_properties(kleinsHTTP-shared PROPERTIES OUTPUT_NAME "kleinsHTTP")
set_target_properties(kleinsHTTP-static PROPERTIES OUTPUT_NAME "kleinsHTTP")

add_compile_definitions(BUILD_VERSION="${CMAKE_PROJECT_VERSION}")

file(READ preheader.h CONTENTS)
file(WRITE libkleinsHTTP.h "${CONTENTS}")
foreach(F ${libhead})
    file(READ "${F}" CONTENTS)
    file(APPEND libkleinsHTTP.h "${CONTENTS} \n")
endforeach()


# set the project name
PROJECT(simpleHelloWorld VERSION 1.0.0)

ADD_EXECUTABLE(simpleHelloWorld examples/simpleHelloWorld/main.cpp)
target_link_libraries(simpleHelloWorld ${CMAKE_SOURCE_DIR}/libkleinsHTTP.a ssl crypto pthread)
set_target_properties(simpleHelloWorld PROPERTIES RUNTIME_OUTPUT_DIRECTORY "examples/simpleHelloWorld/")

add_dependencies(simpleHelloWorld kleinsHTTP-static)


# set the project name
PROJECT(httpsExample VERSION 1.0.0)

ADD_EXECUTABLE(httpsExample examples/httpsExample/main.cpp)
target_link_libraries(httpsExample ${CMAKE_SOURCE_DIR}/libkleinsHTTP.a ssl crypto pthread)
set_target_properties(httpsExample PROPERTIES RUNTIME_OUTPUT_DIRECTORY "examples/httpsExample/")

add_dependencies(httpsExample kleinsHTTP-static)
