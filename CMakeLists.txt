cmake_minimum_required(VERSION 3.20.3)
project(sesame-sdk)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_COMPILER i686-w64-mingw32-gcc)

add_compile_definitions(WIN32)
add_compile_definitions(_DEBUG)
add_compile_definitions(_USE_MATH_DEFINES)
add_compile_definitions(NK_INCLUDE_FIXED_TYPES)
add_compile_definitions(NK_INCLUDE_STANDARD_BOOL)
add_compile_definitions(NK_INCLUDE_DEFAULT_ALLOCATOR)
add_compile_definitions(NK_INCLUDE_STANDARD_IO)
add_compile_definitions(NK_INCLUDE_STANDARD_VARARGS)
add_compile_definitions(NK_INCLUDE_VERTEX_BUFFER_OUTPUT)
add_compile_definitions(NK_INCLUDE_FONT_BAKING)
add_compile_options(-w -g -std=c11 -masm=intel -mmmx -msse2 -fmax-errors=100)

file(GLOB_RECURSE SOURCE_FILES
lib/*/*.c
src/*.c)

add_library(sesame-sdk MODULE ${SOURCE_FILES})

target_link_libraries(sesame-sdk psapi.lib)

target_include_directories(sesame-sdk PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(sesame-sdk PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib)