/* pagetables -- A framework to experiment with page tables
 *
 *    memory.h - Some definitions regarding address spaces.
 *
 * Copyright (C) 2017  Leiden University, The Netherlands.
 */

#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <stdint.h>

/* Like in x86_64, we consider that only 48 bits of the virtual address
 * may be used.
 */
const static uint64_t addressSpaceBits = 48;

/* We restrict the number of bits of physical addresses to 40. This results
 * in a maximum of 1 TB physical memory. We assume that the RAM starts at
 * 4GB in the physical address space.
 */
const static uint64_t maxPhysMemBits = 40;
const static uint64_t maxPhysMem = 1UL << maxPhysMemBits;
const static uint64_t physMemBase = 4UL*1024*1024*1024;

#endif /* __MEMORY_H__ */
