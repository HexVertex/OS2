/* pagetables -- A framework to experiment with page tables
 *
 *    pt-simple.h - A simple, single level page table to serve as example.
 *
 * Copyright (C) 2017  Leiden University, The Netherlands.
 */

#ifndef __PT_SIMPLE__
#define __PT_SIMPLE__

#include "memory.h"
#include "mmu.h"
#include "oskernel.h"

/* Table entry definitions and assorted constants. */

const static uint64_t pageBits = 26; /* 64 MB / page */
const static uint64_t pageSize = 1UL << pageBits;

struct __attribute__ ((__packed__)) TableEntry
{
  uint8_t valid : 1;
  uint8_t read : 1;
  uint8_t write : 1;

  uint16_t reserved : 15;

  uint16_t physicalPage : 14;
};


/* MMU and kernel classes to be implemented. */

class SimpleMMU : public MMU
{
  public:
    SimpleMMU();
    virtual ~SimpleMMU();

    virtual bool performTranslation(const uint64_t vPage,
                                    uint64_t &pPage) override;
};

class SimpleOSKernel : public OSKernel
{
  private:
    TableEntry *table;

  public:
    SimpleOSKernel(MMU &mmu);
    virtual ~SimpleOSKernel();

    virtual void pageFaultHandler(const uint64_t faultAddr);

    /* Disallow objects from being copied, since it has a pointer member. */
    SimpleOSKernel(const SimpleOSKernel &kernel) = delete;
    void operator=(const SimpleOSKernel &kernel) = delete;
};

#endif /* __PT_SIMPLE__ */
