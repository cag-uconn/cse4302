/**
 * University of Connecticut
 * CSE 4302: Computer Architecture
 * Fall 2026
 *
 * Programming Assignment 1: NonPipelined Simulator + Data Cache
 *
 * riscv-uconn: sim_cache.h
 *
 * DO NOT MODIFY THIS FILE
 */

#pragma once

#include "sim_core.h"

int dcache_lookup(unsigned int addr_mem);
void dcache_update(unsigned int addr_mem, int way);
