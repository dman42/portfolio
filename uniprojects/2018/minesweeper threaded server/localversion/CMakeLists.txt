cmake_minimum_required(VERSION 3.10)
project(cab403 C)

include_directories(/usr/local/lib)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

add_executable(Client client.c communication.c communication.h)
add_executable(Server server.c communication.c communication.h)
add_executable(Minetest minetest.c communication.h)