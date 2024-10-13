#!/bin/bash

echo "3) Testing invalid argument value..."

echo "./Task3 123asdadqwe"
./Task3 123asdadqwe > /dev/null 2>&1
if [ $? -ne 0 ]; then
  echo "Test passed: Incorrect input handled correctly"
else
  echo "Test failed: Program did not handle incorrect input"
fi


echo "=================================================================="