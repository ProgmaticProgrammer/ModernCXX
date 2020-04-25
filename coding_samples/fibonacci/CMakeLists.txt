cmake_minimum_required(VERSION 3.10)

project(fibonacci VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(CTest)

if (CMAKE_VERSION VERSION_LESS 3.2)
    set(UPDATE_DISCONNECTED_IF_AVAILABLE "")
else()
    set(UPDATE_DISCONNECTED_IF_AVAILABLE "UPDATE_DISCONNECTED 1")
endif()

include(DownloadProject.cmake)
download_project(PROJ                googletest
                 GIT_REPOSITORY      https://github.com/google/googletest.git
                 GIT_TAG             release-1.10.0
                 ${UPDATE_DISCONNECTED_IF_AVAILABLE}
)

# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})

# When using CMake 2.8.11 or later, header path dependencies
# are automatically added to the gtest and gmock targets.
# For earlier CMake versions, we have to explicitly add the
# required directories to the header search path ourselves.

include_directories("${gtest_SOURCE_DIR}/include"
                    "${gmock_SOURCE_DIR}/include")


list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/lib" "C:/Users/Owner/Desktop/ASL/boost_libraries")

include(BuildType.cmake)

# Trivial example using gtest and gmock
add_executable(fibonacci src/main.cpp)
target_include_directories(fibonacci PUBLIC ${EXTRA_INCLUDES})

add_executable(fibonacci_test test/test.cpp)
target_include_directories(fibonacci_test PUBLIC ${EXTRA_INCLUDES})
target_link_libraries(fibonacci_test gtest_main)
						  
enable_testing()						  
add_test(NAME fibonacci_test COMMAND fibonacci_test)

