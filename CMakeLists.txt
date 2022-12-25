cmake_minimum_required(VERSION 2.8)

project(ProtoGUIParser CXX)

include(GNUInstallDirs)

set(CMAKE_CXX_FLAGS         "-std=c++17 -Wall -Wextra -Wconversion")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

find_package(wxWidgets REQUIRED COMPONENTS std propgrid)
include(${wxWidgets_USE_FILE})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${wxWidgets_CXX_FLAGS}")

file(GLOB SOURCE_FILES src/*.cc)
add_executable(protoGUIParser ${SOURCE_FILES})
target_link_libraries(protoGUIParser ${wxWidgets_LIBRARIES})
