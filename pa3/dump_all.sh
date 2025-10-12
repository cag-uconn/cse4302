#!/bin/bash
rm pa3_out.txt
ASSEMBLEDTEST_DIR=assembled_tests
for ASSEMBLEDTEST in $ASSEMBLEDTEST_DIR/*;
do
    echo "--------------------------------------- $ASSEMBLEDTEST (WITHOUT FWD) (WITHOUT OOO)---------------------------------------" >> pa3_out.txt
    ./simulator ./$ASSEMBLEDTEST 0 0 >> pa3_out.txt
	echo "Done with $ASSEMBLEDTEST (FWD ENBL)"
	echo " " >> pa3_out.txt
	echo "--------------------------------------- $ASSEMBLEDTEST (WITHOUT FWD) (WITHOUT OOO)---------------------------------------" >> pa3_out.txt
	echo " " >> pa3_out.txt
	echo "--------------------------------------- $ASSEMBLEDTEST (WITH FWD) (WITHOUT OOO)---------------------------------------" >> pa3_out.txt
    ./simulator ./$ASSEMBLEDTEST 1 0 >> pa3_out.txt
	echo "Done with $ASSEMBLEDTEST (FWD DISBL)"
	echo " " >> pa3_out.txt
	echo "--------------------------------------- $ASSEMBLEDTEST (WITH FWD) (WITHOUT OOO)---------------------------------------" >> pa3_out.txt
	echo " " >> pa3_out.txt

	echo "--------------------------------------- $ASSEMBLEDTEST (WITHOUT FWD) (WITH OOO)---------------------------------------" >> pa3_out.txt
    ./simulator ./$ASSEMBLEDTEST 0 1 >> pa3_out.txt
	echo "Done with $ASSEMBLEDTEST (FWD ENBL)"
	echo " " >> pa3_out.txt
	echo "--------------------------------------- $ASSEMBLEDTEST (WITHOUT FWD) (WITH OOO)---------------------------------------" >> pa3_out.txt
	echo " " >> pa3_out.txt
	echo "--------------------------------------- $ASSEMBLEDTEST (WITH FWD) (WITH OOO)---------------------------------------" >> pa3_out.txt
    ./simulator ./$ASSEMBLEDTEST 1 1 >> pa3_out.txt
	echo "Done with $ASSEMBLEDTEST (FWD DISBL)"
	echo " " >> pa3_out.txt
	echo "--------------------------------------- $ASSEMBLEDTEST (WITH FWD) (WITH OOO)---------------------------------------" >> pa3_out.txt
	echo " " >> pa3_out.txt

	
done
