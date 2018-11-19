#!/bin/sh

CONFIG=Debug
VCPKG_ROOT=$HOME/.vcpkg

cmake \
    -H. \
    -Bbuild \
    -DCMAKE_BUILD_TYPE=$CONFIG \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=YES \
    -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake \
    -Wno-dev

ln -sf ./build/compile_commands.json .
