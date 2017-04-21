#
# pagetables -- A framework to experiment with page tables
#
# Copyright (C) 2017  Leiden University, The Netherlands.
#

CXX = c++
CXXFLAGS = -Wall -Weffc++ -std=c++14 -O3

# Enable the line below to log all memory accesses to stderr
#CXXFLAGS += -DLOG_MEM_ACCESS

OBJS = tracereader.o main.o mmu.o oskernel.o pt-simple.o
HEADERS = tracereader.h memory.h mmu.h oskernel.h pt-simple.h

all:		pagetables

pagetables:	$(OBJS)
		$(CXX) $(CXXFLAGS) -o $@ $^

%.o:		%.cc $(HEADERS)
		$(CXX) $(CXXFLAGS) -c $<

clean:
		rm -f pagetables
		rm -f *o
