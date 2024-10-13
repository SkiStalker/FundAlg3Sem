#!/bin/bash

echo "3) Testing invalid argument count..."
echo "./Task2"
./Task2 > /dev/null 2>&1
if [ $? -ne 0 ]; then
  echo "Test passed: Incorrect number of arguments (no arguments provided)"
else
  echo "Test failed: Program did not handle missing arguments"
fi

echo "./Task2 arg1"
./Task2 arg1 > /dev/null 2>&1
if [ $? -ne 0 ]; then
  echo "Test passed: Incorrect number of arguments (only 1 argument provided)"
else
  echo "Test failed: Program did not handle insufficient arguments"
fi

echo "=================================================================="