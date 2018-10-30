cmake_minimum_required (VERSION 3.8)

# Target
add_executable(Alpha
    Helpers.h
    Assert.h
    Registry.cpp Registry.h Registry.inl
    Alpha.cpp
)

# Compile options
# NOTE: Using target_compile_options instead of target_compile_features because of clang-tidy.
if (MSVC)
    target_compile_options(Alpha PRIVATE
        /std:c++17 # C++ standard
        /EHsc- # Disable exceptions (not sure if the flag is correct)
    )
else()
    target_compile_options(Alpha PRIVATE
        -std=c++17 # C++ standard
        -fno-exceptions # Disable exceptions
    )
endif()

# OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(Alpha PRIVATE OpenGL::GL)

# SDL2
find_package(SDL2 CONFIG REQUIRED)
target_include_directories(Alpha PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(Alpha PRIVATE ${SDL2_LIBRARIES})

# glew
find_package(GLEW REQUIRED)
target_link_libraries(Alpha PRIVATE GLEW::GLEW)

# glm
find_package(glm CONFIG REQUIRED)
target_link_libraries(Alpha PRIVATE glm)

# clang-tidy
if (CMAKE_CXX_CLANG_TIDY)
    set(ALPHA_CXX_CLANG_TIDY
        ${CMAKE_CXX_CLANG_TIDY}
        -extra-arg=-std=c++17
        -fix
    )
    set_target_properties(Alpha PROPERTIES CXX_CLANG_TIDY "${ALPHA_CXX_CLANG_TIDY}")
endif()

# cppcheck
if (CMAKE_CXX_CPPCHECK)
    set(ALPHA_CXX_CPPCHECK
        ${CMAKE_CXX_CPPCHECK}
        --library=std
        --library=sdl
        #--suppressions-list=${CMAKE_CURRENT_SOURCE_DIR}/CppCheckSuppressions.txt
    )
    set_target_properties(Alpha PROPERTIES CXX_CPPCHECK "${ALPHA_CXX_CPPCHECK}")
endif()
