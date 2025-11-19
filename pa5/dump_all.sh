#!/bin/bash
rm pa5_out.txt
ASSEMBLEDTEST_DIR=assembled_tests
for ASSEMBLEDTEST in $ASSEMBLEDTEST_DIR/*;
do
	echo "--------------------------------------- $ASSEMBLEDTEST (NT PRED) (CACHE DISBL)---------------------------------------" >> pa5_out.txt
    ./simulator ./$ASSEMBLEDTEST 0 0 >> pa5_out.txt
	echo "Done with $ASSEMBLEDTEST (NT PRED) (CACHE DISBL)"
	echo " " >> pa5_out.txt
	echo "--------------------------------------- $ASSEMBLEDTEST (NT PRED) (CACHE DISBL)---------------------------------------" >> pa5_out.txt
	echo " " >> pa5_out.txt
	echo "--------------------------------------- $ASSEMBLEDTEST (NT PRED) (CACHE ENBL)---------------------------------------" >> pa5_out.txt
    ./simulator ./$ASSEMBLEDTEST 0 1 >> pa5_out.txt
	echo "Done with $ASSEMBLEDTEST (NT PRED) (CACHE ENBL)"
	echo " " >> pa5_out.txt
	echo "--------------------------------------- $ASSEMBLEDTEST (NT PRED) (CACHE ENBL)---------------------------------------" >> pa5_out.txt
	echo " " >> pa5_out.txt
	echo "--------------------------------------- $ASSEMBLEDTEST (T PRED) (CACHE DISBL)---------------------------------------" >> pa5_out.txt
	./simulator ./$ASSEMBLEDTEST 1 0 >> pa5_out.txt
	echo "Done with $ASSEMBLEDTEST (T PRED) (CACHE DISBL)"
	echo " " >> pa5_out.txt
	echo "--------------------------------------- $ASSEMBLEDTEST (T PRED) (CACHE DISBL)---------------------------------------" >> pa5_out.txt
	echo " " >> pa5_out.txt
	echo "--------------------------------------- $ASSEMBLEDTEST (T PRED) (CACHE ENBL)---------------------------------------" >> pa5_out.txt
	./simulator ./$ASSEMBLEDTEST 1 1 >> pa5_out.txt
	echo "Done with $ASSEMBLEDTEST (T PRED) (CACHE ENBL)"
	echo " " >> pa5_out.txt
	echo "--------------------------------------- $ASSEMBLEDTEST (T PRED) (CACHE ENBL)---------------------------------------" >> pa5_out.txt
	echo " " >> pa5_out.txt

	
done

