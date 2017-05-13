# Copyright (c) 2017 Martin Moene
#
# https://github.com/martinmoene/ring-span-lite
#
# This code is licensed under the MIT License (MIT).

cmake_minimum_required( VERSION 2.8.12 )

project( ring_span_lite )

option( RING_SPAN_LITE_COLOURISE "Colourise test output" OFF )

enable_testing()

add_subdirectory( test )
#add_subdirectory( example )

# end of file
