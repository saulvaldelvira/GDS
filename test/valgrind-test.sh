#!/bin/bash

if ! type valgrind &> /dev/null; then
    echo "You need to install valgrind"
    exit 1
fi

if ! ls ./bin/*.out &> /dev/null ; then
        make test NO-RUN=true
fi
for t in ./bin/*.out
do
        valgrind --leak-check=yes --error-exitcode=1  "$t"
        if [ $? -ne 0 ] ; then
                exit $?
        fi
done
