#!/bin/bash

make clean ; make CC=gcc FLAGS="-fsanitize=address,undefined,leak -Werror"

failures=""
export failures
find . -name '*.c' |
    while read file
    do
	  cppcheck -j$(nproc) \
	     --enable=style,performance,portability \
	     --error-exitcode=1 \
	     --suppress=constVariable \
	     --suppress=constParameter \
	     --suppress=constParameterPointer\
	     --suppress=constVariablePointer \
	     --std=c99 \
	     $file
	  if [ $? -ne 0 ] ; then
	      failures += "$file,"
	  fi
    done

if [[ $failures != "" ]] ; then
    echo "[CPPCHECK] Failed on: $failures" ;
else
    echo "[CPPCHECK] Finished with no errors" ;
fi
