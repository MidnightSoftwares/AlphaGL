#!/bin/sh

CONFIG=Debug

cmake \
    --build ./build \
    --config $CONFIG
