cmake_minimum_required(VERSION 3.10)
project(create_map C)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/../../cmake)

message(STATUS ${CMAKE_SOURCE_DIR}/../../cmake)

set(CMAKE_GENERATOR_PLATFORM x64)

if (WIN32)
    set(SDL2_DIR "${CMAKE_SOURCE_DIR}/lib/win32/SDL2")
    set(SDL2_IMAGE_PATH "${CMAKE_SOURCE_DIR}/lib/win32/SDL2_image")
    set(SDL2_MIXER_PATH "${CMAKE_SOURCE_DIR}/lib/win32/SDL2_mixer")
    set(SDL2_TTF_PATH "${CMAKE_SOURCE_DIR}/lib/win32/SDL2_ttf")
elseif(APPLE)
    list(APPEND CMAKE_FRAMEWORK_PATH "${CMAKE_SOURCE_DIR}/lib/apple")
endif()

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra)
endif()

message(STATUS "[message]: System '${CMAKE_SYSTEM_NAME}', Platform: '${CMAKE_GENERATOR_PLATFORM}'")

message(STATUS "[message]: Add 'SDL2' package...")
find_package(SDL2 REQUIRED)

message(STATUS "[message]: Add 'include/'(*.h) directory...")
include_directories(${CMAKE_SOURCE_DIR}/../../include)

message(STATUS "[message]: Configure to create an executable...")
add_executable(${PROJECT_NAME} create_map.c)

message(STATUS "[message]: Defining include directories...")
target_include_directories(
    ${PROJECT_NAME} SYSTEM
    PRIVATE ${SDL2_INCLUDE_DIR}
)

message(STATUS "[message]: Linking libraries...")
target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE ${SDL2_LIBRARY}
)

set(EXECUTABLE_DESTINATION "${CMAKE_SOURCE_DIR}/create_map")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E make_directory "${EXECUTABLE_DESTINATION}"
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> "${EXECUTABLE_DESTINATION}/"
)
