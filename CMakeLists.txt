
cmake_minimum_required(VERSION 3.23)

project(JpegCodec)

include(FetchContent)
include(GenerateExportHeader)

find_package(Git REQUIRED)
if(NOT GIT_FOUND)
    message(FATAL_ERROR "Git is required but not found.")
endif()
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
FetchContent_Declare(
    libjpeg-turbo
    GIT_REPOSITORY https://github.com/libjpeg-turbo/libjpeg-turbo.git
    GIT_TAG        98c458381f0938b2cea222f5b278b4ced6f70805
)
FetchContent_GetProperties(libjpeg-turbo)
function(ApplyPatch project_name)
    find_package(Git REQUIRED)
    set(patch_file "${CMAKE_CURRENT_SOURCE_DIR}/${project_name}.patch")
    set(patch_stamp "${CMAKE_CURRENT_BINARY_DIR}/${project_name}_patch_applied.stamp")

    if(NOT EXISTS "${patch_file}")
        message(FATAL_ERROR "Patch file ${patch_file} does not exist.")
    endif()
    if(EXISTS "${patch_stamp}")
        message(STATUS "Patch for ${project_name} already applied (stamp found)")
        return()
    endif()
    execute_process(
        COMMAND ${GIT_EXECUTABLE} -C ${${project_name}_SOURCE_DIR} apply ${patch_file}
        RESULT_VARIABLE error
        OUTPUT_VARIABLE output
        ERROR_VARIABLE error_output
    )
    if(error)
        message(FATAL_ERROR "Failed to apply patch to ${project_name}:\n${error_output}")
    else()
        message(STATUS "Successfully applied patch to ${project_name}")
        file(WRITE "${patch_stamp}" "Patch applied on ${CMAKE_CURRENT_TIME}")
    endif()
endfunction()
FetchContent_GetProperties(libjpeg-turbo)
cmake_policy(SET CMP0169 OLD)
if(NOT libjpeg-turbo_POPULATED)
    FetchContent_Populate(libjpeg-turbo)
    if(NOT libjpeg-turbo_POPULATED)
        message(FATAL_ERROR "Failed to populate libjpeg-turbo")
    endif()
    if(NOT EXISTS "${libjpeg-turbo_SOURCE_DIR}")
        message(FATAL_ERROR "libjpeg-turbo source directory not found: ${libjpeg-turbo_SOURCE_DIR}")
    endif()
    ApplyPatch(libjpeg-turbo)
    add_subdirectory(${libjpeg-turbo_SOURCE_DIR} ${libjpeg-turbo_BINARY_DIR})
endif()
add_library(JpegCodecObj    OBJECT)
add_library(JpegCodecShared SHARED)
add_library(JpegCodecStatic STATIC)
target_compile_definitions(
    JpegCodecShared
    PUBLIC
        JPEG_CODEC_SHARED
    PRIVATE
        JPEG_CODEC_BUILD
)
target_compile_features(JpegCodecObj PUBLIC cxx_std_17)
target_sources(JpegCodecObj
    PUBLIC
    FILE_SET HEADERS BASE_DIRS
        headers
    FILES
        headers/jpeg-codec-cpp/jpeg.h
    PRIVATE
        sources/jpeg.cc
)
target_link_libraries(
    JpegCodecObj
    PUBLIC
    JpegCodecCoreObj
    $<IF:$<TARGET_EXISTS:JpegCodecSimdObj>,JpegCodecSimdObj,>
)
foreach(target JpegCodecShared JpegCodecStatic)
    target_link_libraries(
        ${target}
        PUBLIC
            JpegCodecObj
            JpegCodecCoreObj
            $<IF:$<TARGET_EXISTS:JpegCodecSimdObj>,JpegCodecSimdObj,>
    )
endforeach()
