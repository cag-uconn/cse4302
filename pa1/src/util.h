/**
 * University of Connecticut
 * CSE 4302: Computer Architecture
 * Fall 2025
 * 
 * Programming Assignment 1: NonPipelined Simulator
 * 
 * riscv-uconn: util.h
 * 
 * DO NOT MODIFY THIS FILE  
 */

#pragma once

#include <stdio.h>

void rdump_file_columns(FILE* file, unsigned columns);
void rdump();
void rdump_pt();
void mdump();
void mdump_modified();
void inst_dump(const char stage[], const unsigned int inst);
void getInstStr(int op, int f3, int f7, char *buffer);
int getDec(char *bin);