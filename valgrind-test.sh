#!/bin/bash
if [ -d "./bin" ]; then
status=0
for t in ./bin/*
do
valgrind --leak-check=yes --error-exitcode=1  --log-fd=1 2>/dev/null "$t"
status=$?
if [ $status -ne 0 ] ;
then
exit $status
fi
done
fi
