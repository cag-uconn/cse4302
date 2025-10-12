#!/bin/bash

# This script is privided to you to assemble all unit tests for you.
# 1) First ensure that that your assembler is compiled
# 2) Then, run "$ bash assemble_all.sh". 
# A new directory "assembled tests will be generated, populated with all the binary tests.
# Note: "$ make clean" will remove these tests

if [ ! -d assembled_tests ]; then
	echo "Creating \"assembled_tests\" directory..."
	mkdir assembled_tests
fi

if [ ! -f ../assembler/assembler ]; then
	echo "Error: Assembler is not compiled."
	exit
fi

for test in unittests/*.asm; do
	echo "Assembling test \"$test\"..."
	testname=$(basename $test .asm)
	../assembler/assembler $test assembled_tests/$testname.out > /dev/null 2>&1
done
echo "Done!"