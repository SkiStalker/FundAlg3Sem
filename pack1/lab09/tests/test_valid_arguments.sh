#!/bin/bash

echo "2) Testing valid arguments..."

echo "./9 10 50"
./9 10 50
if [[ $? -ne 0 ]]; then
    echo "Test failed: program should succeed with valid arguments."
else
    echo "Test passed: program succeeded as expected." 
fi

echo "==================================================================="