/* pagetables -- A framework to experiment with page tables
 *
 *    main.cc - Program driver
 *
 * Copyright (C) 2017  Leiden University, The Netherlands.
 */

#include <iostream>

#include "tracereader.h"
#include "pt-simple.h"

int
main(int argc, char **argv)
{
  SimpleMMU mmu;
  SimpleOSKernel kernel(mmu);

  try
    {
      TraceReader reader(std::cin);

      while (not reader.eof())
        {
          MemAccess access;

          reader >> access;
          mmu.processMemAccess(access);
        }
    }
  catch (TraceFileParseError &error)
    {
      std::cerr << "Parse error: " << error.what() << std::endl;
      return -1;
    }
  catch (std::runtime_error &error)
    {
      std::cerr << "Runtime error: " << error.what() << std::endl;
      return -1;
    }
  catch (...)
    {
      std::cerr << "Unknown exception occurred." << std::endl;
      return -1;
    }

  return 0;
}
