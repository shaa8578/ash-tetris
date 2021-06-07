cmake_minimum_required(VERSION 3.5)

project(ASH_TETRIS VERSION 0.0.1 LANGUAGES CXX)

#-------------------------------------------------------------------------------
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
  message(FATAL_ERROR "The build directory can not equal source!!!")
endif()

if(NOT (CMAKE_BUILD_TYPE))
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Тип сборки" FORCE)
  message(STATUS "Set default build type: ${CMAKE_BUILD_TYPE}")
else()
  message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
endif()

if(NOT DEFINED UNIX)
  message(FATAL_ERROR "Building and working project is possible only Linux")
endif()

#-------------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE ON)
set(BUILD_SHARED_LIBS ON)

#-------------------------------------------------------------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin CACHE PATH
  "Путь до исполняемых файлов сборки")

#-------------------------------------------------------------------------------
set(CURSES_NEED_NCURSES OFF)
find_package(Curses REQUIRED)

#-------------------------------------------------------------------------------
set(SOURCES_FILES
  src/main.cpp
  src/game_play.cpp
  src/figure.cpp
  src/figures/square.cpp
  src/figure_ext.cpp
  src/figures/line.cpp
  src/figures/n_figure.cpp
  src/figures/u_figure.cpp
  src/figures/t_figure.cpp
  src/collision_model.cpp
)

set(HEADERS_FILES
  src/game_play.h
  src/figure.h
  src/figures/square.h
  src/figure_ext.h
  src/figures/line.h
  src/figures/n_figure.h
  src/figures/u_figure.h
  src/figures/t_figure.h
  src/collision_model.h
)
#-------------------------------------------------------------------------------
set(PROJECT_LABEL ash-tetris)
add_executable(${PROJECT_LABEL} WIN32
  ${SOURCES_FILES}
  ${HEADERS_FILES}
)

target_compile_features(${PROJECT_LABEL} PRIVATE
    cxx_auto_type
    cxx_generic_lambdas
)

target_compile_options(${PROJECT_LABEL} PRIVATE
  -Wall
  -Wextra
  -Wpedantic
  -Wno-deprecated-copy
  -Wnonnull
  # -save-temps
)

target_compile_definitions(${PROJECT_LABEL} PRIVATE
  $<$<NOT:$<CONFIG:Debug>>:USING_SRAND>
)

target_include_directories(${PROJECT_LABEL} PRIVATE src)

target_link_libraries(${PROJECT_LABEL} PRIVATE ${CURSES_LIBRARIES})
