cmake_minimum_required(VERSION 3.2)
project(LB4)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -lpthread  -lboost_serialization -lboost_program_options -lboost_system")

#set(SOURCE_FILES server/server.cpp)
#add_executable(LB4 ${SOURCE_FILES} server/server.cpp)
#add_executable(LB4 ${SOURCE_FILES} client/client.hpp client/client.cpp )
#add_executable(LB4 ${SOURCE_FILES} server/server_asio.cpp tests/test_server.cpp)
add_executable(LB4 ${SOURCE_FILES} tests/test_server.cpp)

#add_executable(LB4 ${SOURCE_FILES} server/server.cpp headers/types.hpp server/task.hpp headers/serializ.cpp)
#add_executable(LB4 ${SOURCE_FILES} examples/source_file.cpp headers/types.hpp server/work.cpp server/work.cpp client/client.hpp)
#add_executable(LB4 ${SOURCE_FILES} examples/source_file.cpp client/client.cpp headers/headers.h)
#add_executable(LB4 ${SOURCE_FILES} server/server.cpp examples/source_file.cpp)