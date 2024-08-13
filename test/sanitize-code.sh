#!/bin/sh

if ! which cppcheck &> /dev/null ; then
    echo "You need to install cppcheck"
    exit 1
fi

make clean
make CC=gcc FLAGS="-fsanitize=address,undefined,leak -Werror"

if cppcheck --help | grep check-level ; then
    CHECK_LEVEL=--check-level=exhaustive
fi

find src -name '*.c' |
    while read file
    do
	  cppcheck -j$(nproc) \
	     --enable=style,performance,portability \
	     --error-exitcode=1 \
	     --suppress=constParameterPointer \
	     --std=c99 \
         $CHECK_LEVEL \
	     $file
    done

