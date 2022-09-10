#!/bin/bash
ARGS="$1"
ROOTDIR="$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )"
CONFIGS="release"

test "$ARGS" = "install" && {
    for CONFIG in $CONFIGS; do
        cd "$ROOTDIR/build/$CONFIG" && make install
    done

    exit 0
}


for CONFIG in $CONFIGS; do
    BUILDDIR=$ROOTDIR/build/$CONFIG

    mkdir -p $BUILDDIR
    cd $BUILDDIR

    cmake -DCMAKE_BUILD_TYPE=$CONFIG $ROOTDIR
    make #-j24
done

cd "$ROOTDIR"
cp "$ROOTDIR/build/release/compile_commands.json" .

echo Finished creation of: gum-opengl
