cmake_minimum_required(VERSION 3.25)

project(ChickenFlock VERSION 1.0.0 LANGUAGES CXX)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)

file(GLOB_RECURSE CHICKEN_FLOCK_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

file(GLOB_RECURSE CHICKEN_FLOCK_HEADERS CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

set(CHICKEN_FLOCK_INCLUDE_DIRS)
foreach(header IN LISTS CHICKEN_FLOCK_HEADERS)
    get_filename_component(header_directory "${header}" DIRECTORY)
    list(APPEND CHICKEN_FLOCK_INCLUDE_DIRS "${header_directory}")
endforeach()
list(REMOVE_DUPLICATES CHICKEN_FLOCK_INCLUDE_DIRS)

add_executable(ChickenFlock
    ${CHICKEN_FLOCK_SOURCES}
    ${CHICKEN_FLOCK_HEADERS}
)

target_compile_features(ChickenFlock PRIVATE cxx_std_17)
target_include_directories(ChickenFlock PRIVATE ${CHICKEN_FLOCK_INCLUDE_DIRS})

target_link_libraries(ChickenFlock PRIVATE
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_mixer::SDL2_mixer
)

if(WIN32)
    target_link_libraries(ChickenFlock PRIVATE SDL2::SDL2main)
endif()

if(MSVC)
    target_compile_options(ChickenFlock PRIVATE /W4 /permissive- /utf-8)
else()
    target_compile_options(ChickenFlock PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_custom_command(TARGET ChickenFlock POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E make_directory
        "$<TARGET_FILE_DIR:ChickenFlock>/assets/images"
        "$<TARGET_FILE_DIR:ChickenFlock>/assets/audio/music"
        "$<TARGET_FILE_DIR:ChickenFlock>/assets/audio/sfx"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "${PROJECT_SOURCE_DIR}/assets/images"
        "$<TARGET_FILE_DIR:ChickenFlock>/assets/images"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "${PROJECT_SOURCE_DIR}/assets/audio/music"
        "$<TARGET_FILE_DIR:ChickenFlock>/assets/audio/music"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "${PROJECT_SOURCE_DIR}/assets/audio/sfx"
        "$<TARGET_FILE_DIR:ChickenFlock>/assets/audio/sfx"
    COMMAND_EXPAND_LISTS
    VERBATIM
)

if(WIN32)
    add_custom_command(TARGET ChickenFlock POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_if_different
            $<TARGET_RUNTIME_DLLS:ChickenFlock>
            "$<TARGET_FILE_DIR:ChickenFlock>"
        COMMAND_EXPAND_LISTS
        VERBATIM
    )
endif()
