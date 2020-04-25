cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(Drawable VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# add the executable
add_executable(Drawable main.cxx)
target_include_directories(Drawable PUBLIC ${PROJECT_SOURCE_DIR})

enable_testing()

# does the application run
add_test(NAME Runs COMMAND Drawable)