if (GCC OR Clang)
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic # Non-standard C++ is used
        -Wshadow # Variable declaration shadows one from a parent context
        -Wnon-virtual-dtor # Class with virtual functions has a non-virtual destructor.
        -Wold-style-cast # C-style casts
        -Wcast-align # Potential performance problem casts
        -Wunused # Anything being unused
        -Woverloaded-virtual # Overload (not override) a virtual function
        -Wconversion # Type conversions that may lose data
        -Wsign-conversion # Sign conversions
        -Wmisleading-indentation # Indentation implies blocks where blocks do not exist
        -Wduplicated-cond # If / else chain has duplicated conditions
        -Wduplicated-branches # If / else branches have duplicated code
        -Wlogical-op # Kogical operations being used where bitwise were probably wanted
        -Wnull-dereference # Null dereference is detected
        -Wuseless-cast # Perform a cast to the same type
        -Wdouble-promotion # Float is implicit promoted to double
        -Wformat=2 # Security issues around functions that format output (ie printf)
    )

    if (Clang)
        add_compile_options(
            -Wlifetime # Shows object lifetime issues
        )
    endif()
elseif (MSVC)
    # Set /W4 flag
    if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()

    add_compile_options(
        /permissive- # Enforces standards conformance
        /w14242 # 'identfier': conversion from 'type1' to 'type1', possible loss of data
        /w14254 # 'operator': conversion from 'type1:field_bits' to 'type2:field_bits', possible loss of data
        /w14263 # 'function': member function does not override any base class virtual member function
        /w14265 # 'classname': class has virtual functions, but destructor is not virtual instances of this class may not be destructed correctly
        /w14287 # 'operator': unsigned/negative constant mismatch
        /we4289 # nonstandard extension used: 'variable': loop control variable declared in the for-loop is used outside the for-loop scope
        /w14296 # 'operator': expression is always 'boolean_value'
        /w14311 # 'variable': pointer truncation from 'type1' to 'type2'
        /w14545 # expression before comma evaluates to a function which is missing an argument list
        /w14546 # function call before comma missing argument list
        /w14547 # 'operator': operator before comma has no effect; expected operator with side-effect
        /w14549 # 'operator': operator before comma has no effect; did you intend 'operator'?
        /w14555 # expression has no effect; expected expression with side-effect
        /w14619 # pragma warning: there is no warning number 'number'
        /w14640 # Enable warning on thread un-safe static member initialization
        /w14826 # Conversion from 'type1' to 'type_2' is sign-extended. This may cause unexpected runtime behavior.
        /w14905 # Wide string literal cast to 'LPSTR'
        /w14906 # String literal cast to 'LPWSTR'
        /w14928 # Illegal copy-initialization; more than one user-defined conversion has been implicitly applied

        /wd4996 # Suppress C4996
    )

    # Set correct /MD or /MT flags depending on vcpkg triplet
    set(MSVC_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_MINSIZEREL
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_RELWITHDEBINFO
    )
    if (${VCPKG_TARGET_TRIPLET} MATCHES "static")
        foreach (flag ${MSVC_CXX_FLAGS})
            if (${flag} MATCHES "/MD")
                string(REGEX REPLACE "/MD" "/MT" ${flag} "${${flag}}")
            endif()
        endforeach()
        set(VCPKG_CRT_LINKAGE "static")
        set(VCPKG_LIBRARY_LINKAGE "static")
    else()
        foreach (flag ${MSVC_CXX_FLAGS})
            if (${flag} MATCHES "/MT")
                string(REGEX REPLACE "/MT" "/MD" ${flag} "${${flag}}")
            endif()
        endforeach()
        set(VCPKG_CRT_LINKAGE "dynamic")
        set(VCPKG_LIBRARY_LINKAGE "dynamic")
    endif()
endif()
