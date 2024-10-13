#!/bin/bash

echo "1) Testing valid arguments..."

echo "./Task3 -q 0.001 1 5 6"
./Task3 -q 0.001 1 5 6
if [[ $? -ne 0 ]]; then
    echo "Test failed: program should succeed with valid arguments."
else
    echo "Test passed: program succeeded as expected." 
fi

echo "./Task3 -m 10 2"
./Task3 -m 10 2
if [[ $? -ne 0 ]]; then
    echo "Test failed: program should succeed with valid arguments."
else
    echo "Test passed: program succeeded as expected." 
fi

echo "./Task3 -t 0.001 3 4 5"
./Task3 -t 0.001 3 4 5
if [[ $? -ne 0 ]]; then
    echo "Test failed: program should succeed with valid arguments."
else
    echo "Test passed: program succeeded as expected." 
fi
echo "==================================================================="