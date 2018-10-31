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
    )

    # Disable exceptions
    if (CMAKE_CXX_FLAGS MATCHES "/EHsc")
        string(REGEX REPLACE "/EHsc" "/EHsc-" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHSC-")
    endif()
else()
    target_compile_options(Alpha PRIVATE
        -std=c++17 # C++ standard
        -fno-exceptions # Disable exceptions
    )
endif()

# Compile definitions
if (CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_definitions(Alpha PRIVATE _DEBUG)

    if (MSVC)
        # Disable MSVC verified iterators for better debug performance
        target_compile_definitions(Alpha PRIVATE _ITERATOR_DEBUG_LEVEL=0)
    endif()
else()
    target_compile_definitions(Alpha PRIVATE NDEBUG)
endif()

# OpenGL
find_package(OpenGL REQUIRED)
target_link_libraries(Alpha PRIVATE OpenGL::GL)

# SDL2
find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(Alpha PRIVATE SDL2::SDL2main SDL2::SDL2-static)

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
