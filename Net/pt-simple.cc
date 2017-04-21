/* pagetables -- A framework to experiment with page tables
 *
 *    pt-simple.h - A simple, single level page table to serve as example.
 *
 * Copyright (C) 2017  Leiden University, The Netherlands.
 */

#include "pt-simple.h"

/*
 * Helper functions for generating assertions and forming page table
 * entries and addresses.
 */

static inline void
initPageTableEntry(TableEntry &entry, const uintptr_t address)
{
  entry.physicalPage = address >> pageBits;
  entry.read = 1;
  entry.valid = 1;
}

/*
 * MMU part. The MMU simply performs a walk of the page table and
 * attempts the translation from a virtual to a physical address.
 */

SimpleMMU::SimpleMMU()
{
}

SimpleMMU::~SimpleMMU()
{
}

bool
SimpleMMU::performTranslation(const uint64_t vPage,
                              uint64_t &pPage)
{
  /* Strip off the sign-extension in the high 16 bits */
  const int entry = vPage & ((1UL << (addressSpaceBits - pageBits)) - 1);

  const TableEntry *table = reinterpret_cast<const TableEntry *>(root);
  if (not table[entry].valid)
    return false;

  pPage = table[entry].physicalPage;
  return true;
}


/* OSkernel part. The OS kernel is in charge of actually allocating and
 * organizing the page tables.
 */

const static int entries = 1 << (addressSpaceBits - pageBits);

SimpleOSKernel::SimpleOSKernel(MMU &mmu)
  : OSKernel(mmu, pageSize), table(nullptr)
{
  table = new TableEntry[entries];
  bytesAllocated += entries * sizeof(TableEntry);

  for (int i = 0; i < entries; ++i)
    table[i].valid = 0;

  using std::placeholders::_1;
  mmu.initialize(reinterpret_cast<uintptr_t>(table), pageBits,
                 std::bind(&OSKernel::pageFaultHandler, this, _1 ));
}

SimpleOSKernel::~SimpleOSKernel()
{
  delete[] table;
}

void
SimpleOSKernel::pageFaultHandler(const uint64_t faultAddr)
{
  logPageFault(faultAddr);

  /* Strip off the sign-extension in the high 16 bits */
  uint64_t addr(faultAddr & ((1UL << addressSpaceBits) - 1));
  const uint64_t vPage = (addr & ~(pageSize-1));
  const int entry = vPage / pageSize;

  initPageTableEntry(table[entry], allocator.allocatePage());

  logPageMapping(faultAddr & ~(pageSize - 1),
                 (uint64_t)table[entry].physicalPage << pageBits);
}
