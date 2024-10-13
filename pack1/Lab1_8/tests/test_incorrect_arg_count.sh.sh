#!/bin/bash

echo "3) Testing invalid argument count..."
echo "./Task8"
./Task8 > /dev/null 2>&1
if [ $? -ne 0 ]; then
  echo "Test passed: Incorrect number of arguments (no arguments provided)"
else
  echo "Test failed: Program did not handle missing arguments"
fi

echo "./Task8 /home/admin1/FundAlg3Sem/Laba1/Lab1_8/resources/input.txt"
./Task8 /home/admin1/FundAlg3Sem/Laba1/Lab1_8/resources/input.txt > /dev/null 2>&1
if [ $? -ne 0 ]; then
  echo "Test passed: Incorrect number of arguments (only 1 argument provided)"
else
  echo "Test failed: Program did not handle insufficient arguments"
fi

echo "=================================================================="