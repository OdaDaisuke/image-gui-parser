cmake_minimum_required(VERSION 2.8)

project(ImageGUIParser CXX)

include(GNUInstallDirs)

set(CMAKE_CXX_FLAGS         "-std=c++17 -Wall -Wextra -Wconversion")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")

find_package(Boost     REQUIRED COMPONENTS system)
find_package(wxWidgets REQUIRED COMPONENTS std propgrid)
include(${wxWidgets_USE_FILE})

include_directories(${Boost_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${wxWidgets_CXX_FLAGS}")

file(GLOB SOURCE_FILES src/*.cc)
add_executable(imageGUIParser ${SOURCE_FILES})
target_link_libraries(imageGUIParser ${wxWidgets_LIBRARIES} ${Boost_LIBRARIES})
