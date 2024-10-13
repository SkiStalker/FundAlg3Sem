#!/bin/bash

echo "2) Testing valid arguments..."

echo "./Task8 /home/admin1/FundAlg3Sem/Laba1/Lab1_8/resources/input.txt  /home/admin1/FundAlg3Sem/Laba1/Lab1_8/resources/output.txt"
./Task8 /home/admin1/FundAlg3Sem/Laba1/Lab1_8/resources/input.txt  /home/admin1/FundAlg3Sem/Laba1/Lab1_8/resources/output.txt
if [[ $? -ne 0 ]]; then
    echo "Test failed: program should succeed with valid arguments."
else
    echo "Test passed: program succeeded as expected." 
fi

echo "==================================================================="