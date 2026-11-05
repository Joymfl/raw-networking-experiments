cmake_minimum_required(VERSION 4.4.4)
project(networking VERSION 0.1 LANGUAGES C)

add_executable(client client/main.c)
add_executable(server server/main.c)
add_executable(test addrinfo/main.c)
