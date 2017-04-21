/* pagetables -- A framework to experiment with page tables
 *
 *    oskernel.h - OS kernel component
 *
 * Copyright (C) 2017  Leiden University, The Netherlands.
 */

#ifndef __OSKERNEL_H__
#define __OSKERNEL_H__

#include "mmu.h"
#include "memory.h"

/* An overly simple "physical memory allocator". We simply allocate
 * pages in sequence starting at a base address, since we do not have
 * any specific requirements. Given that memory is never released
 * in this simulator, we don't bother about releasing pages here.
 */
class PhysMemAllocator
{
  private:
    const uint64_t baseAddress;
    const uint64_t pageSize;
    uint64_t seq;

  public:
    PhysMemAllocator(const uint64_t baseAddress,
                     const uint64_t pageSize);

    uint64_t allocatePage(void);
};

/* The OS kernel is only concerned with allocating physical memory and
 * creating page table mappings in response to page faults. We do not
 * deal with page permissions, virtual address space maps, and so on to
 * keep things simple.
 */
class OSKernel
{
  protected:
    PhysMemAllocator allocator;

    int nPageFaults;
    uint64_t bytesAllocated;

    void logPageFault(const uint64_t faultAddr);
    void logPageMapping(const uint64_t virtualAddr,
                        const uint64_t physicalAddr);

  public:
    OSKernel(MMU &mmu, const uint64_t pageSize);
    virtual ~OSKernel();

    /* An implementation of the page fault handler should add a virtual
     * to physical mapping to the page fault for a virtual page that
     * contains the specified fault address.
     */
    virtual void pageFaultHandler(const uint64_t faultAddr) = 0;
};

#endif /* __OSKERNEL_H__ */
