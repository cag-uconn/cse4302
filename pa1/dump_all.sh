#!/bin/bash
# Run all assembled tests with a chosen cache associativity.
# Usage:
#   ./dump_all.sh ASSOCIATIVITY
#   ASSOCIATIVITY: 1 = direct mapped, 2 = 2-way, 4 = 4-way
#
# Example:
#   ./dump_all.sh 1
#   ./dump_all.sh 2
#   ./dump_all.sh 4

if [ $# -ne 1 ]; then
    echo "usage: ./dump_all.sh ASSOCIATIVITY"
    echo "  ASSOCIATIVITY: 1 = direct mapped, 2 = 2-way, 4 = 4-way"
    exit 1
fi

ASSOC=$1
if [ "$ASSOC" != "1" ] && [ "$ASSOC" != "2" ] && [ "$ASSOC" != "4" ]; then
    echo "[ERROR] ASSOCIATIVITY must be 1, 2, or 4."
    exit 1
fi

rm -f pa1_out.txt
ASSEMBLEDTEST_DIR=assembled_tests
for ASSEMBLEDTEST in $ASSEMBLEDTEST_DIR/*;
do
    echo "--------------------------------------- $ASSEMBLEDTEST (assoc=$ASSOC) ---------------------------------------" >> pa1_out.txt
    ./simulator ./$ASSEMBLEDTEST $ASSOC >> pa1_out.txt
    echo "Done with $ASSEMBLEDTEST (assoc=$ASSOC)"
    echo " " >> pa1_out.txt
done
