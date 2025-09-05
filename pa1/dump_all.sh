#!/bin/bash
rm pa1_out.txt
ASSEMBLEDTEST_DIR=assembled_tests
for ASSEMBLEDTEST in $ASSEMBLEDTEST_DIR/*;
do
    echo "--------------------------------------- $ASSEMBLEDTEST ---------------------------------------" >> pa1_out.txt
    ./simulator ./$ASSEMBLEDTEST >> pa1_out.txt
	echo "Done with $ASSEMBLEDTEST"
	echo " " >> pa1_out.txt
done