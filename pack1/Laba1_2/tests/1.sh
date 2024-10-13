#!/bin/bash

echo "2) Testing valid arguments..."

echo "./Task2 0.0001"
./Task2 0.0001
if [[ $? -ne 0 ]]; then
    echo "Test failed: program should succeed with valid arguments."
else
    echo "Test passed: program succeeded as expected." 
fi

echo "==================================================================="