#!/bin/bash

echo "3) Testing invalid argument value..."

echo "./Task4 -d njnknk"
./Task4 -d Lab1_4/resources/4.txt > /dev/null 2>&1
if [ $? -ne 0 ]; then
  echo "Test passed: Incorrect input handled correctly"
else
  echo "Test failed: Program did not handle incorrect input"
fi


echo "=================================================================="