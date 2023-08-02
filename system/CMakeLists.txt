# cmake_minimum_required(VERSION 3.22)

# project(TableNavSystem)

# add_executable(TableNavSystem main.cpp)
# # add_library(SocketLib STATIC main.cpp)
# add_library(SocketLib STATIC "")
# add_subdirectory(Lib)
# # target_link_libraries(TableNavSystem SocketLib)

cmake_minimum_required(VERSION 3.22)
project(table_nav_system)

# Set the C++ standard version
set(CMAKE_CXX_STANDARD 11)

# Add the main executable target
add_executable(table_nav_system main.cpp)

# Include directories for your project
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Lib/Socket)
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Lib/Utils)

# Add source files for the Lib/Socket target
add_library(Socket
    Lib/Socket/SocketServer.cpp
    Lib/Socket/SocketClient.cpp
    Lib/Socket/Socket.cpp
    Lib/Socket/TcpSocket.h
)

# Add source files for the Lib/Utils target
# add_library(Utils
#     Lib/Utils/lib_source_3.cpp
#     Lib/Utils/lib_source_4.cpp
#     Lib/Utils/lib_header.h
# )

# Link libraries to the main executable target
target_link_libraries(table_nav_system Socket)

# Optionally, you can add additional compiler flags if needed
# For example, uncomment the following line to enable warnings:
# target_compile_options(your_project_name PRIVATE -Wall -Wextra)