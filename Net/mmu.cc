/* pagetables -- A framework to experiment with page tables
 *
 *    mmu.cc - Memory Management Unit component
 *
 * Copyright (C) 2017  Leiden University, The Netherlands.
 */

#include "mmu.h"
#include "memory.h"

#ifdef LOG_MEM_ACCESS
#  include <iostream>
#endif

MMU::MMU()
  : pageBits(0), pageSize(0), root(0x0), pageFaultHandler()
{
}

MMU::~MMU()
{
}

void
MMU::initialize(const uintptr_t _root,
                const uint8_t _pageBits,
                std::function<void(uintptr_t)> _pageFaultHandler)
{
  root = _root;
  pageBits = _pageBits;
  pageSize = 1UL << pageBits;
  pageFaultHandler = _pageFaultHandler;
}

void
MMU::processMemAccess(const MemAccess &access)
{
#ifdef LOG_MEM_ACCESS
  std::cerr << "MMU: memory access: " << access << std::endl;
#endif /* LOG_MEM_ACCESS */

  uint64_t pAddr = 0x0;
  while (not getTranslation(access, pAddr))
    {
      pageFaultHandler(access.addr);
    }

#ifdef LOG_MEM_ACCESS
  std::cerr << "MMU: translated virtual "
            << std::hex << std::showbase << access.addr
            << " to physical " << pAddr << std::endl;
#endif /* LOG_MEM_ACCESS */
}

uint64_t
MMU::makePhysicalAddr(const MemAccess &access, const uint64_t pPage)
{
  uint64_t pAddr = pPage << pageBits;
  pAddr |= access.addr & (pageSize - 1);

  return pAddr;
}

bool
MMU::getTranslation(const MemAccess &access, uint64_t &pAddr)
{
  const uint64_t vPage = access.addr >> pageBits;
  uint64_t pPage = 0;

  if (performTranslation(vPage, pPage))
    {
      pAddr = makePhysicalAddr(access, pPage);
      return true;
    }

  return false;
}
