project(examples)
cmake_minimum_required(VERSION 2.8)
INCLUDE_DIRECTORIES(../src)

add_executable(${PROJECT_NAME}-tcpserver tcpserver.cpp)
target_link_libraries(${PROJECT_NAME}-tcpserver sockcpp)

add_executable(${PROJECT_NAME}-udpserver udpserver.cpp)
target_link_libraries(${PROJECT_NAME}-udpserver sockcpp)
