!/bin/bash

echo "2) Testing invalid argument count..."
echo "./Task4"
./Task4 > /dev/null 2>&1
if [ $? -ne 0 ]; then
  echo "Test passed: Incorrect number of arguments (no arguments provided)"
else
  echo "Test failed: Program did not handle missing arguments"
fi

echo "./Task4 -d"
./Task4  -d > /dev/null 2>&1
if [ $? -ne 0 ]; then
  echo "Test passed: Incorrect number of arguments (only 1 argument provided)"
else
  echo "Test failed: Program did not handle insufficient arguments"
fi

echo "=================================================================="