#!/bin/bash
rm pa4_out.txt
ASSEMBLEDTEST_DIR=assembled_tests
for ASSEMBLEDTEST in $ASSEMBLEDTEST_DIR/*;
do
	echo "--------------------------------------- $ASSEMBLEDTEST (WITH FWD) (WITH OOO)---------------------------------------" >> pa4_out.txt
    ./simulator ./$ASSEMBLEDTEST 1 1 >> pa4_out.txt
	echo "Done with $ASSEMBLEDTEST (WITH FWD) (WITH OOO)"
	echo " " >> pa4_out.txt
	echo "--------------------------------------- $ASSEMBLEDTEST (WITH FWD) (WITH OOO)---------------------------------------" >> pa4_out.txt
	echo " " >> pa4_out.txt
done
