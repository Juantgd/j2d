cmake_minimum_required(VERSION 3.16)

project(sdl3_test VERSION 0.0.1 LANGUAGES C CXX)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)

set(SDL_X11_XTEST OFF)

# add_compile_options(-Wall -Wextra -Wpedantic -Wconversion -Wshadow)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>/lib")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release")
endif()

add_subdirectory("third_party/SDL" EXCLUDE_FROM_ALL)
add_subdirectory("third_party/SDL_image" EXCLUDE_FROM_ALL)
add_subdirectory("third_party/SDL_ttf" EXCLUDE_FROM_ALL)

include_directories("${CMAKE_SOURCE_DIR}/include")

add_executable(${PROJECT_NAME} src/main.cc src/app_entry.cc)

target_link_libraries(${PROJECT_NAME} PRIVATE
  SDL3::SDL3
  SDL3_image::SDL3_image
  SDL3_ttf::SDL3_ttf
)
