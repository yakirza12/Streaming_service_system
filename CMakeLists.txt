cmake_minimum_required(VERSION 3.15)
project(Streaming_service_system)

set(CMAKE_CXX_STANDARD 11)

add_executable(Streaming_service_system src/Action.cpp src/Main.cpp src/Session.cpp src/User.cpp src/Watchable.cpp )