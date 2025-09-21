#!/bin/bash
#   This scipt ignores the CPI and CYCLES CIMULATED for your implemented PA.
#   It checks the difference in register values , memory and in the instructions commited
#   This is the syntax : 
#                           diff -I 'TOTAL CYCLES SIMULATED' -I 'AVERAGE CPI'  "file provided"  "output of your dump file" 

diff -I 'TOTAL CYCLES SIMULATED' -I 'AVERAGE CPI' pa2_out_gold_wo_CPI.txt pa2_out.txt