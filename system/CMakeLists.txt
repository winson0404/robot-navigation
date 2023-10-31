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
SET(GCC_COVERAGE_COMPILE_FLAGS "-fno-stack-protector")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")


find_package(PkgConfig)
pkg_check_modules(SERIAL libserial)

# Add the main executable target
add_executable(table_nav_system main.cpp)



# Include directories for your project
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Lib/Socket)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Lib/SerialComm)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Lib/Util)

# Add source files for the Lib/Socket target
add_library(Socket SHARED
    Lib/Socket/SocketServer.cpp
    Lib/Socket/SocketClient.cpp
    Lib/Socket/Socket.cpp
    Lib/Socket/TcpSocket.h
)

add_library(SerialComm SHARED
    Lib/SerialComm/SerialComm.cpp
    Lib/SerialComm/SerialComm.h)

add_library(Util SHARED
    Lib/Util/Input.cpp
    Lib/Util/Connection.cpp
    Lib/Util/PostWork.cpp
    Lib/Util/Utils.h
)


# Add source files for the Lib/Utils target
# add_library(Utils
#     Lib/Utils/lib_source_3.cpp
#     Lib/Utils/lib_source_4.cpp
#     Lib/Utils/lib_header.h
# )

# target_include_directories(table_nav_system ${FOO_INCLUDE_DIR} ${BAR_INCLUDE_DIR} )

# Link libraries to the main executable target
target_link_libraries(Util Socket)
target_link_libraries(Util SerialComm)
target_link_libraries(table_nav_system PRIVATE Util)
# target_link_libraries(table_nav_system PRIVATE Socket)
# target_link_libraries(table_nav_system PRIVATE SerialComm)
target_include_directories(SerialComm PRIVATE ${SERIAL_INCLUDE_DIRS})
target_link_libraries(SerialComm PRIVATE ${SERIAL_LDFLAGS} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(SerialComm PRIVATE ${LibSerial_LIBRARIES})

# Optionally, you can add additional compiler flags if needed
# For example, uncomment the following line to enable warnings:
# target_compile_options(your_project_name PRIVATE -Wall -Wextra)