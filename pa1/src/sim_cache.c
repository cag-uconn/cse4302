/**
 * University of Connecticut
 * CSE 4302: Computer Architecture
 * Fall 2026
 *
 * Programming Assignment 1: NonPipelined Simulator + Data Cache
 *
 * riscv-uconn: sim_cache.c
 *
 * STUDENTS: modify this file only for the cache portion of PA1.
 */

#include "sim_core.h"
#include "sim_stages.h"
#include "sim_cache.h"

/* DATA CACHE FUNCTIONS
 *
 * Geometry (do not redefine here):
 *   cache_size, cache_line_size  - CACHE_SIZE / CACHE_LINE_SIZE in sim_core.h
 *   cache_assoc                  - command line: 1, 2, or 4
 *   num_sets = cache_size / (cache_line_size * cache_assoc)
 *
 * dcache[index].block[way]  - tag / valid
 * dcache[index].lru1    - replacement bits
 *   2-way: lru1 = which way is LRU (0 or 1)
 *   4-way: lru1 = 1-bit split-group LRU
 *
 * dcache_lookup / dcache_update dispatch on cache_assoc.
 * Implement (or replace) the *_DM / *_2_way / *_4_way helpers below.
 */

/* ========================================================================
 * Direct mapped (cache_assoc == 1)
 * ======================================================================== */

static int dcache_lookup_DM(unsigned int addr_mem) {
    // students: implement this function for direct-mapped cache lookup

    // use this for cache_trace in case of a hit
    // if (cache_trace == 1) {
    //     fprintf(fptr_mt, "hit index %d, tag %d, way %d, cur LRU %d", index, tag, 0, -1);
    // }
    
    // use this for cache_trace in case of a miss
    // if (cache_trace == 1) {
    //     fprintf(fptr_mt, "miss index %d, tag %d, cur LRU %d", index, tag, -1);
    // }
    return -1;
}

static void dcache_update_DM(unsigned int addr_mem) {
    // students: implement this function for direct-mapped cache update

    // use this for cache_trace
    // if (cache_trace == 1) {
    //     fprintf(fptr_mt, " new lru %d,", -1);
    // }
}

/* ========================================================================
 * 2-way set associative + 1-bit LRU (lru1)
 *   lru1 == 0 -> way 0 is LRU
 *   lru1 == 1 -> way 1 is LRU
 * After using way w, set lru1 to the other way.
 * ======================================================================== */

static int dcache_lookup_2_way(unsigned int addr_mem) {
    // students: implement this function for 2-way set associative cache lookup

    // use this for cache_trace in case of a hit
    // if (cache_trace == 1) {
    //     fprintf(fptr_mt, "hit index %d, tag %d, way %d, cur LRU %d", index, tag, i, dcache[index].lru1);
    // }

    // use this for cache_trace in case of a miss
    // if (cache_trace == 1) {
    //     fprintf(fptr_mt, "miss index %d, tag %d, cur LRU %d", index, tag, dcache[index].lru1);
    // }
    return -1;
}

static void dcache_update_2_way(unsigned int addr_mem, int line) {
    // students: implement this function for 2-way set associative cache update

    // use this for cache_trace
    // if (cache_trace == 1) {
    //     fprintf(fptr_mt, " new lru %d,", dcache[index].lru1);
    // }
}

/* ========================================================================
 * 4-way set associative + 1-bit split-group LRU (lru1)
 * lru1 == 0 -> upper half (Ways 2/3) becomes LRU half (lru1 = 1)
 * lru1 == 1 -> lower half (Ways 0/1) becomes LRU half (lru1 = 0)
 * Valid ways take priority for eviction over invalid ways
 * Tie-breaker: Both are valid, or both are invalid, then evict the lower-indexed way of the pair
 * ======================================================================== */

static int dcache_lookup_4_way_1bitlru(unsigned int addr_mem) {
    // students: implement this function for 4-way set associative cache lookup

    // use this for cache_trace in case of a hit
    // if (cache_trace == 1) {
    //     fprintf(fptr_mt, "hit index %d, tag %d, way %d, cur LRU %d", index, tag, i, dcache[index].lru1);
    // }

    // use this for cache_trace in case of a miss
    // if (cache_trace == 1) {
    //     fprintf(fptr_mt, "miss index %d, tag %d, cur LRU %d", index, tag, dcache[index].lru1);
    // }
    return -1;
}

static void dcache_update_4_way_1bitlru(unsigned int addr_mem, int line) {
    // students: implement this function for 4-way set associative cache update

    // use this for cache_trace
    // if (cache_trace == 1) {
    //     fprintf(fptr_mt, " new lru %d,", dcache[index].lru1);
    // }
}

/* ========================================================================
 *  Selects implementation from cache_assoc -- DO NOT MODIFY!
 * ======================================================================== */

int dcache_lookup(unsigned int addr_mem) {
    if (cache_assoc == 1) {
        return dcache_lookup_DM(addr_mem);
    } else if (cache_assoc == 2) {
        return dcache_lookup_2_way(addr_mem);
    } else if (cache_assoc == 4) {
        return dcache_lookup_4_way_1bitlru(addr_mem);
    }
    return -1;
}

void dcache_update(unsigned int addr_mem, int way) {
    if (cache_assoc == 1) {
        dcache_update_DM(addr_mem);
    } else if (cache_assoc == 2) {
        dcache_update_2_way(addr_mem, way);
    } else if (cache_assoc == 4) {
        dcache_update_4_way_1bitlru(addr_mem, way);
    }
}

