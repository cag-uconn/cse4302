#!/bin/bash
rm pa2_out.txt
ASSEMBLEDTEST_DIR=assembled_tests
for ASSEMBLEDTEST in $ASSEMBLEDTEST_DIR/*;
do
    echo "--------------------------------------- $ASSEMBLEDTEST (WITHOUT FWD) ---------------------------------------" >> pa2_out.txt
    ./simulator ./$ASSEMBLEDTEST 0 >> pa2_out.txt
	echo "Done with $ASSEMBLEDTEST (FWD ENBL)"
	echo " " >> pa2_out.txt
	echo "--------------------------------------- $ASSEMBLEDTEST (WITHOUT FWD)---------------------------------------" >> pa2_out.txt
	echo " " >> pa2_out.txt
	echo "--------------------------------------- $ASSEMBLEDTEST (WITH FWD) ---------------------------------------" >> pa2_out.txt
    ./simulator ./$ASSEMBLEDTEST 1 >> pa2_out.txt
	echo "Done with $ASSEMBLEDTEST (FWD DISBL)"
	echo " " >> pa2_out.txt
	echo "--------------------------------------- $ASSEMBLEDTEST (WITH FWD)---------------------------------------" >> pa2_out.txt
	echo " " >> pa2_out.txt
done