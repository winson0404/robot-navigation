# cmake_minimum_required(VERSION 3.22)

# project(TableNavSystem)

# add_executable(TableNavSystem main.cpp)
# # add_library(SocketLib STATIC main.cpp)
# add_library(SocketLib STATIC "")
# add_subdirectory(Lib)
# # target_link_libraries(TableNavSystem SocketLib)

cmake_minimum_required(VERSION 3.22)
project(test)

# Set the C++ standard version
set(CMAKE_CXX_STANDARD 11)
SET(GCC_COVERAGE_COMPILE_FLAGS "-fno-stack-protector")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")


# find_package(LibSerial REQUIRED)
# find_package(PkgConfig)
# pkg_check_modules(SERIAL libserial)
# include_directories(${LibSerial_INCLUDE_DIRS})
find_package(PkgConfig)
pkg_check_modules(SERIAL libserial)
# Add the main executable target
add_executable(test main.cpp)


target_include_directories(test PRIVATE ${SERIAL_INCLUDE_DIRS})
target_link_libraries(test PRIVATE ${SERIAL_LDFLAGS} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(test ${LibSerial_LIBRARIES})


# Include directories for your project
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Lib/Socket)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Lib/Util)

# Add source files for the Lib/Socket target
add_library(Socket SHARED
    Lib/Socket/SocketServer.cpp
    Lib/Socket/SocketClient.cpp
    Lib/Socket/Socket.cpp
    Lib/Socket/TcpSocket.h
)

add_library(Util SHARED
    Lib/Util/Input.cpp
    Lib/Util/Connection.cpp
    Lib/Util/PostWork.cpp
    Lib/Util/Utils.h
)


# Add source files for the Lib/Utils target
add_library(Utils
    Lib/Utils/lib_source_3.cpp
    Lib/Utils/lib_source_4.cpp
    Lib/Utils/lib_header.h
)

# target_include_directories(table_nav_system ${FOO_INCLUDE_DIR} ${BAR_INCLUDE_DIR} )

# Link libraries to the main executable target
target_link_libraries(Util Socket)
target_link_libraries(test Util)

# Optionally, you can add additional compiler flags if needed
# For example, uncomment the following line to enable warnings:
# target_compile_options(your_project_name PRIVATE -Wall -Wextra)