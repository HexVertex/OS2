/* pagetables -- A framework to experiment with page tables
 *
 *    tracereader.cc - Class to read valgrind "lackey" memory traces.
 *
 * Copyright (C) 2017  Leiden University, The Netherlands.
 */

#include "tracereader.h"

std::ostream &
operator<<(std::ostream &stream, const MemAccess &access)
{
  switch (access.type)
    {
      case MemAccessType::Instr:
          stream << "Instr ";
          break;

      case MemAccessType::Store:
          stream << "Store ";
          break;

      case MemAccessType::Load:
          stream << "Load  ";
          break;

      case MemAccessType::Modify:
          stream << "Mod   ";
          break;
    }

  stream << std::hex << std::showbase << access.addr << " "
         << std::dec << std::noshowbase
         << (int)access.size;

  return stream;
}


TraceReader::TraceReader(std::istream &input)
  : input(input), lineno(0), eofflag(false), currentAccess()
{
  parseToNextAccess();
}

void
TraceReader::parseType(const std::string &str, MemAccess &access)
{
  if (str[0] == 'I')
    access.type = MemAccessType::Instr;
  else if (str[1] == 'S')
    access.type = MemAccessType::Store;
  else if (str[1] == 'L')
    access.type = MemAccessType::Load;
  else if (str[1] == 'M')
    access.type = MemAccessType::Modify;
  else
    throw TraceFileParseError(lineno, "invalid memory access type '" +
                              str.substr(0, 2) + "'");
}

bool
TraceReader::parseLine(const std::string &line, MemAccess &access)
{
  if (line.substr(0, 2) == "==")
    return false;

  /* A line containing a memory access must be at least 6 characters
   * long "IL ", followed by an address, comma and size all at least
   * one character in size.
   */
  if (line.size() < 6)
    return false;

  parseType(line, access);

  const size_t pos = line.find(',', 3);
  if (pos == 3 or pos == line.size() - 1)
    throw TraceFileParseError(lineno, "malformed address/size pair");

  access.addr = std::stoull(line.substr(3, pos - 3), nullptr, 16);
  access.size = std::stoul(line.substr(pos + 1));

  return true;
}

void
TraceReader::parseToNextAccess()
{
  while (not input.eof())
    {
      std::string line;
      std::getline(input, line);
      ++lineno;

      if (parseLine(line, currentAccess))
        return;
    }

  eofflag = true;
}

TraceReader &
TraceReader::operator>>(MemAccess &access)
{
  access = currentAccess;
  parseToNextAccess();
  return *this;
}
