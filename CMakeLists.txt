cmake_minimum_required (VERSION 3.8)

project(Alpha
    LANGUAGES CXX
)

include(cmake/Compiler.cmake)
include(cmake/ClangTidy.cmake)
include(cmake/Cppcheck.cmake)

add_subdirectory(Alpha)
