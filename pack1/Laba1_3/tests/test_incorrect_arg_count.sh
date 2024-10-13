#!/bin/bash

echo "2) Testing invalid argument count..."
echo "./Task3"
./Task3 > /dev/null 2>&1
if [ $? -ne 0 ]; then
  echo "Test passed: Incorrect number of arguments (no arguments provided)"
else
  echo "Test failed: Program did not handle missing arguments"
fi

echo "./Task3 -q arg1 arg2"
./Task3 -q arg1 arg2 > /dev/null 2>&1
if [ $? -ne 0 ]; then
  echo "Test passed: Incorrect number of arguments (only 1 argument provided)"
else
  echo "Test failed: Program did not handle insufficient arguments"
fi


echo "./Task3 -m arg1"
./Task3 -m arg1 > /dev/null 2>&1
if [ $? -ne 0 ]; then
  echo "Test passed: Incorrect number of arguments (only 1 argument provided)"
else
  echo "Test failed: Program did not handle insufficient arguments"
fi

echo "./Task3 -t arg1 arg2"
./Task3 -t arg1 arg2 > /dev/null 2>&1
if [ $? -ne 0 ]; then
  echo "Test passed: Incorrect number of arguments (only 1 argument provided)"
else
  echo "Test failed: Program did not handle insufficient arguments"
fi



echo "=================================================================="