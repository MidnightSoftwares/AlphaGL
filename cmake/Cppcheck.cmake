# find_program(CPPCHECK_EXE NAMES cppcheck)

if (CPPCHECK_EXE)
    set(CMAKE_CXX_CPPCHECK
        ${CPPCHECK_EXE}
        --enable=all
        --inconclusive
        --force
        --inline-suppr
    )

    if (MSVC)
        list(APPEND CMAKE_CXX_CPPCHECK
            --library=windows
            --template=vs
        )
    else()
        list(APPEND CMAKE_CXX_CPPCHECK
            --library=gnu
            --template=gcc
        )
    endif()
endif()
