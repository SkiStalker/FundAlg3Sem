#!/bin/bash

echo "3) Testing invalid argument count..."

echo "./9"
./9
if [[ $? -eq 0 ]]; then
    echo "Test failed: program should not succeed with no arguments."
else
    echo "Test passed: program failed as expected."
fi

echo "./9 123"
./9 123
if [[ $? -eq 0 ]]; then
    echo "Test failed: program should not succeed with one argument."
else
    echo "Test passed: program failed as expected."
fi

echo "============================================================="
