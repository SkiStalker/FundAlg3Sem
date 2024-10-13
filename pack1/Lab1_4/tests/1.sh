#!/bin/bash

echo "1) Testing valid arguments..."

echo "./Task4 -i /home/admin1/FundAlg3Sem/Laba1/Lab1_4/resources/4.txt"
./Task4 -i /home/admin1/FundAlg3Sem/Laba1/Lab1_4/resources/4.txt
if [[ $? -ne 0 ]]; then
    echo "Test failed: program should succeed with valid arguments."
else
    echo "Test passed: program succeeded as expected." 
fi


echo "==================================================================="