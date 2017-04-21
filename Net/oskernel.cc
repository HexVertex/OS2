/* pagetables -- A framework to experiment with page tables
 *
 *    oskernel.cc - OS kernel component
 *
 * Copyright (C) 2017  Leiden University, The Netherlands.
 */

#include "oskernel.h"

#include <iostream>


PhysMemAllocator::PhysMemAllocator(const uint64_t baseAddress,
                                   const uint64_t pageSize)
  : baseAddress(baseAddress), pageSize(pageSize), seq(0)
{
  std::cerr << "OS: system memory @ "
            << std::hex << std::showbase << baseAddress
            << " page size of " << pageSize << " bytes." << std::endl;
}

uint64_t
PhysMemAllocator::allocatePage(void)
{
  if (seq * pageSize >= maxPhysMem)
    throw std::runtime_error("Max. physical memory reached.");

  return baseAddress + seq++ * pageSize;
}


OSKernel::OSKernel(MMU &mmu, const uint64_t pageSize)
  : allocator(physMemBase, pageSize), nPageFaults(0), bytesAllocated(0)
{
}

OSKernel::~OSKernel()
{
  std::cerr << std::dec << std::endl
            << "Statistics:" << std::endl
            << "# handled page faults: " << nPageFaults << std::endl
            << "# bytes allocated for page tables: "
            << bytesAllocated << std::endl;
}

void
OSKernel::logPageFault(const uint64_t faultAddr)
{
  std::cerr << "OS: handling page fault for address "
            << std::hex << std::showbase << faultAddr << std::endl;
  nPageFaults++;
}

void
OSKernel::logPageMapping(const uint64_t virtualAddr,
                         const uint64_t physicalAddr)
{
  std::cerr << std::hex << std::showbase
            << "OS: virtual page " << virtualAddr
            << " has been mapped to physical page "
            << physicalAddr
            << std::endl;
}
