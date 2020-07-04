cmake_minimum_required( VERSION 3.14 FATAL_ERROR )

set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM.cmake")
set(CPM_DOWNLOAD_VERSION 0.27)

if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION} AND CPM_VERSION STREQUAL CPM_DOWNLOAD_VERSION))
    message(STATUS "Downloading CPM.cmake")
    file(DOWNLOAD https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake ${CPM_DOWNLOAD_LOCATION})
endif()

include(${CPM_DOWNLOAD_LOCATION})

project(COMP-371-Project
        VERSION 0.0.1
        DESCRIPTION "Project for COMP 371"
        LANGUAGES CXX)

# set everything up for c++ 17 features
set(CMAKE_CXX_STANDARD 17)
# Don't add this line if you will try_compile with boost.
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} main.cpp shader.h camera.h axis.cpp axis.h)

#glfw
CPMAddPackage(
        NAME glfw
        VERSION 3.3.2
        GITHUB_REPOSITORY glfw/glfw
        GIT_TAG 3.3.2
        OPTIONS
        "GLFW_BUILD_EXAMPLES OFF"
        "GLFW_BUILD_TESTS OFF"
)

#glew
add_definitions(-DGLEW_STATIC)
CPMAddPackage(
        NAME glew
        VERSION 2.1.0
        GITHUB_REPOSITORY Perlmint/glew-cmake
        GIT_TAG glew-cmake-2.1.0
        OPTIONS
        "glew-cmake_BUILD_STATIC ON"
        "glew-cmake_BUILD_SHARED OFF"
)

#glm
CPMAddPackage(
        NAME glm
        VERSION 0.9.9.8
        GITHUB_REPOSITORY g-truc/glm
        GIT_TAG 0.9.9.8
)

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wno-dev -Wall -Wextra -pedantic -Werror)
endif()

target_link_libraries(${PROJECT_NAME}
        PUBLIC
            glfw
            libglew_static
            glm
)

message(STATUS test)
file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})