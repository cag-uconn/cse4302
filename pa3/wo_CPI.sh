#!/bin/bash

# File to modify
FILE="pa3_out.txt"

# Replace occurrences of TOTAL INSTRUCTIONS COMMITTED and TOTAL CYCLES SIMULATED
sed -i 's/TOTAL CYCLES SIMULATED: [0-9]\+/TOTAL CYCLES SIMULATED: x/' "$FILE"
sed -i 's/AVERAGE CPI: [0-9]\+/AVERAGE CPI: x/' "$FILE"
sed -i 's/AVERAGE CPI: x.[0-9]\+/AVERAGE CPI: x/' "$FILE"

echo "Modifications completed!"
