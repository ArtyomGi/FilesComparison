cmake_minimum_required(VERSION 3.5)

project(FilesComparison LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(FilesComparison
  main.cpp
  FilesComp.h
  FilesComp.cpp
  Dir.h
  Dir.cpp
  ComparisonWorker.h
  ComparisonWorker.cpp
  ComparisonResult.h
)

target_link_libraries(FilesComparison PRIVATE Qt5::Widgets)
