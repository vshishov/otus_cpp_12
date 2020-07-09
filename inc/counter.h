#pragma once

#include <cstddef>
#include <iostream>
#include <string>

namespace otus {
    
class Counters
{
public:
  Counters(std::string a_strName)
    : m_strName{a_strName}
    , lineCounter{0}
    , blockCounter{0}
    , commandCounter{0}
  {}

  friend std::ostream& operator<< (std::ostream& a_osOut, const Counters& a_counters)
  {
    a_osOut << a_counters.m_strName << ": ";

    if (a_counters.lineCounter > 0) {
      a_osOut << a_counters.lineCounter << " line(s), ";
    }
    a_osOut << a_counters.blockCounter << " block(s), " << a_counters.commandCounter << " command(s)";

    return a_osOut;
  }

public:
  std::string m_strName;
  std::size_t lineCounter;
  std::size_t blockCounter;
  std::size_t commandCounter;
};

} // otus::
