#!/bin/bash

SHADERC="../../../cmake-build-debug/_deps/bgfx-build/cmake/bgfx/shaderc"
BGFX_INCLUDE="../../../cmake-build-debug/_deps/bgfx-src/bgfx/src"
OUT="compiled"

mkdir -p $OUT

# Vertex shader - Metal macOS
$SHADERC \
    -f vs_simple.sc \
    -o $OUT/vs_simple.bin \
    --type vertex \
    --platform osx \
    -p metal \
    --verbose \
    -i $BGFX_INCLUDE

# Fragment shader - Metal macOS
$SHADERC \
    -f fs_simple.sc \
    -o $OUT/fs_simple.bin \
    --type fragment \
    --platform osx \
    -p metal \
    --verbose \
    -i $BGFX_INCLUDE

echo "Done"