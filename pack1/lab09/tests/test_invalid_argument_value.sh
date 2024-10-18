#!/bin/bash

echo "1) Testing invalid argument value..."

echo "./9 123asdadqwe"
./9 123asdadqwe
if [[ $? -eq 0 ]]; then
    echo "Test failed: program should not succeed with invalid argument."
else
    echo "Test passed: program failed as expected."
fi

echo "=================================================================="
