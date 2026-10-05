cmake_minimum_required(VERSION 3.25)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    if(DEFINED ENV{VCPKG_ROOT})
        set(_vcpkg_root "$ENV{VCPKG_ROOT}")
    elseif(WIN32 AND DEFINED ENV{USERPROFILE})
        set(_vcpkg_root "$ENV{USERPROFILE}/dev/vcpkg")
    endif()

    if(DEFINED _vcpkg_root AND
       EXISTS "${_vcpkg_root}/scripts/buildsystems/vcpkg.cmake")
        set(CMAKE_TOOLCHAIN_FILE
            "${_vcpkg_root}/scripts/buildsystems/vcpkg.cmake"
            CACHE FILEPATH "vcpkg CMake toolchain"
        )
    endif()
endif()

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE OR
   NOT EXISTS "${CMAKE_TOOLCHAIN_FILE}")
    message(FATAL_ERROR
        "ChickenFlock uses vcpkg manifest mode for SDL dependencies. "
        "Set VCPKG_ROOT to your vcpkg checkout or configure "
        "-DCMAKE_TOOLCHAIN_FILE=<vcpkg-root>/scripts/buildsystems/vcpkg.cmake."
    )
endif()

if(WIN32 AND NOT DEFINED VCPKG_TARGET_TRIPLET)
    set(VCPKG_TARGET_TRIPLET "x64-windows" CACHE STRING "vcpkg target triplet")
endif()

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

file(GLOB CHICKEN_FLOCK_SOURCE_GROUPS LIST_DIRECTORIES true
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*"
)
set(CHICKEN_FLOCK_INCLUDE_DIRS)
foreach(source_group IN LISTS CHICKEN_FLOCK_SOURCE_GROUPS)
    if(IS_DIRECTORY "${source_group}")
        list(APPEND CHICKEN_FLOCK_INCLUDE_DIRS "${source_group}")
    endif()
endforeach()

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
