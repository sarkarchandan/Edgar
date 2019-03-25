cmake_minimum_required(VERSION 3.1...3.13)
if(${CMAKE_VERSION} VERSION_LESS 3.13)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()
project(Edgar VERSION 1.0 DESCRIPTION "An over-simplified prototypical implementation for databases and CRUD operations" LANGUAGES CXX)
include(CTest)

if(NOT TARGET gtest OR NOT TARGET gtest_main OR NOT TARGET gmock OR NOT TARGET gmock_main)

  include(FetchContent)

  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.8.1
  )
  FetchContent_GetProperties(googletest)

  if(NOT googletest_POPULATED)
    FetchContent_Populate(googletest)
    add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
  endif()

endif()
set(SOURCES
    src/Container.cpp
    src/TransactionFactory.cpp)

add_library(Edgar SHARED ${SOURCES})
target_include_directories(Edgar PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(Edgar PUBLIC cxx_std_14)

option(PACKAGE_TEST "Build for tests" ON)
if(PACKAGE_TEST)
  enable_testing()
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif()
