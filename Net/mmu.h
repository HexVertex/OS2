/* pagetables -- A framework to experiment with page tables
 *
 *    mmu.h - Memory Management Unit component
 *
 * Copyright (C) 2017  Leiden University, The Netherlands.
 */

#ifndef __MMU_H__
#define __MMU_H__

#include <functional>

#include "tracereader.h" /* for MemAccess */

class MMU
{
  protected:
    uint8_t pageBits;
    uint64_t pageSize;

    uintptr_t root;
    std::function<void(uintptr_t)> pageFaultHandler;

  public:
    MMU();
    virtual ~MMU();

    void initialize(const uintptr_t root,
                    const uint8_t _pageBits,
                    std::function<void(uintptr_t)> pageFaultHandler);
    void processMemAccess(const MemAccess &access);

    uint64_t makePhysicalAddr(const MemAccess &access, const uint64_t pPage);
    bool getTranslation(const MemAccess &access, uint64_t &pAddr);

    /* An implementation of the "performTranslation" method should translate
     * the given virtual page *number* to a physical page number. We
     * consider a page number to be the address with the page offset
     * shifted away. This is done in terms of page numbers to simplify
     * implementation of a TLB.
     */
    virtual bool performTranslation(const uint64_t vPage,
                                    uint64_t &pPage) = 0;
};


#endif /* __MMU_H__ */
