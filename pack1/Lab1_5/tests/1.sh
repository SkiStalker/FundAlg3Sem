#!/bin/bash

echo "2) Testing valid arguments..."

echo "./Task5 0.0001 0.5"
./Task5 0.0001 0.5
if [[ $? -ne 0 ]]; then
    echo "Test failed: program should succeed with valid arguments."
else
    echo "Test passed: program succeeded as expected." 
fi

echo "==================================================================="