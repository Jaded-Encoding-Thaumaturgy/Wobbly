# Description

There are two executables, Wibbly and Wobbly.

Wibbly gathers metrics and creates project files that Wobbly can open. (Wobbly can also open video files directly.)

Wobbly can be used to filter a video per scene, and/or to improve upon VFM and VDecimate's decisions.

# Compilation

The usual steps work::
    ./autogen.sh
    ./configure
    make

Requirements: - A C++23 compiler.

    - Qt 5.4 or newer (just qtbase).

    - VapourSynth r32 or newer.

# License

The code itself is available under the ISC license.
