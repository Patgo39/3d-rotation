cmake_minimum_required(VERSION 3.28)

project(3d-rotation
        VERSION 1.0
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Curses REQUIRED)

add_executable(${PROJECT_NAME} 
    src/main.cpp
  )

target_include_directories(${PROJECT_NAME} PRIVATE ${CURSES_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${CURSES_LIBRARIES})
