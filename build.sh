#!/bin/bash

ROOTDIR="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"

CONFIGS="release"


for CONFIG in $CONFIGS; do

    BUILDDIR=$ROOTDIR/build/$CONFIG

    mkdir -p $BUILDDIR

    cd $BUILDDIR

    cmake -DCMAKE_BUILD_TYPE=$CONFIG $ROOTDIR

    make -j24
    mv ./src/libGumOpenGL.a ../../libGumOpenGL.a 2> /dev/null
done

cd "$ROOTDIR"
cp "$ROOTDIR/build/release/compile_commands.json" .

echo Finished creation of: gum-opengl
