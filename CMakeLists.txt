cmake_minimum_required(VERSION 3.10)
project(raycast_engine C)

set(RENDER_API "OpenGL")
# set(RENDER_API "Vulkan")

set(CMAKE_C_STANDARD 11)

if (WIN32)
    set(SDL2_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/win32/SDL2/cmake")
    set(SDL2_image_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/win32/SDL2_image/cmake")
    set(SDL2_mixer_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/win32/SDL2_mixer/cmake")
    set(SDL2_ttf_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib/win32/SDL2_ttf/cmake")
elseif(APPLE)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
    list(APPEND CMAKE_FRAMEWORK_PATH "${CMAKE_SOURCE_DIR}/lib/apple")
endif()

if (MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra)
endif()

message(STATUS "[message]: Add 'SDL2' package...")
find_package(SDL2 REQUIRED)

message(STATUS "[message]: Add 'SDL2_image' package...")
find_package(SDL2_image REQUIRED)

message(STATUS "[message]: Add 'SDL2_mixer' package...")
find_package(SDL2_mixer REQUIRED)

message(STATUS "[message]: Add 'SDL2_ttf' package...")
find_package(SDL2_ttf REQUIRED)

message(STATUS "[message]: Add 'OpenGL' package...")
find_package(OpenGL REQUIRED)

message(STATUS "[message]: Add 'VulkanAPI' package...")
find_package(Vulkan REQUIRED)

message(STATUS "[message]: Add 'include/'(*.h) directory...")
include_directories(${CMAKE_SOURCE_DIR}/include)

message(STATUS "[message]: Add 'src/'(*.c) directory...")
file(GLOB SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.c")

if (${RENDER_API} STREQUAL  "OpenGL")
    add_definitions(-DOPENGL)
    message(STATUS "[message]: Setting up OpenGL...")
    list(APPEND SOURCES "src/render/render_opengl.c")
elseif (${RENDER_API} STREQUAL "Vulkan")
    add_definitions(-DVULKAN)
    message(STATUS "[message]: Setting up Vulkan...")
    list(APPEND SOURCES "src/render/render_vulkan.c")
endif()

message(STATUS "[message]: Configure to create an executable for '${CMAKE_SYSTEM_NAME}'...")
add_executable(${PROJECT_NAME} ${SOURCES})

message(STATUS "[message]: Defining include directories...")
target_include_directories(
    ${PROJECT_NAME} SYSTEM
    PRIVATE ${SDL2_INCLUDE_DIR}
    PRIVATE ${SDL2_IMAGE_INCLUDE_DIRS}
    PRIVATE ${SDL2_MIXER_INCLUDE_DIRS}
    PRIVATE ${SDL2_TTF_INCLUDE_DIRS}
    Vulkan::Headers
)

message(STATUS "[message]: Linking libraries...")

if (WIN32)
    target_link_libraries(
        ${PROJECT_NAME}
        PRIVATE SDL2::SDL2
        PRIVATE SDL2::SDL2main
        PRIVATE SDL2_image::SDL2_image
        PRIVATE SDL2_mixer::SDL2_mixer
        PRIVATE SDL2_ttf::SDL2_ttf
        OpenGL::GL
        Vulkan::Vulkan
    )
elseif(APPLE)
    target_link_libraries(
        ${PROJECT_NAME}
        PRIVATE ${SDL2_LIBRARY}
        PRIVATE ${SDL2_IMAGE_LIBRARIES}
        PRIVATE ${SDL2_MIXER_LIBRARIES}
        PRIVATE ${SDL2_TTF_LIBRARIES}
        OpenGL::GL
        Vulkan::Vulkan
    )
endif()

set(EXECUTABLE_DESTINATION "${CMAKE_SOURCE_DIR}/engine")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E make_directory ${EXECUTABLE_DESTINATION}
)

if (WIN32)
    get_target_property(SDL2_DLL_LOCATION SDL2::SDL2 IMPORTED_LOCATION)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy ${SDL2_DLL_LOCATION} ${EXECUTABLE_DESTINATION}
    )

    get_target_property(SDL2_IMAGE_DLL_LOCATION SDL2_image::SDL2_image IMPORTED_LOCATION)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy ${SDL2_IMAGE_DLL_LOCATION} ${EXECUTABLE_DESTINATION}
    )

    get_target_property(SDL2_MIXER_DLL_LOCATION SDL2_mixer::SDL2_mixer IMPORTED_LOCATION)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy ${SDL2_MIXER_DLL_LOCATION} ${EXECUTABLE_DESTINATION}
    )

    get_target_property(SDL2_TTF_DLL_LOCATION SDL2_ttf::SDL2_ttf IMPORTED_LOCATION)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy ${SDL2_TTF_DLL_LOCATION} ${EXECUTABLE_DESTINATION}
    )
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/assets/" ${EXECUTABLE_DESTINATION}/assets
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PROJECT_NAME}> ${EXECUTABLE_DESTINATION}
)