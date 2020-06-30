#pragma once

#include "observer.h"
#include "command.h"
#include <counter.h>

#include <iostream>
#include <string>
#include <map>

namespace otus {

class Commander : public BaseObservable<CommandBlock>
{
public:
  static std::shared_ptr<Commander> Create(const std::string& a_strName, std::size_t a_szBlockSize, std::ostream& a_osMetricsOut = std::cout);
  ~Commander();

  void ProcessLine(const std::string& a_strId, const std::string& a_strLine);

  void Stop(const std::string& a_strId);

private:
  Commander(const std::string& a_strName, std::size_t a_szBlockSize, std::ostream& a_osMetricsOut = std::cout);

  void Flush();

private:
  std::ostream& m_osMetricsOut;
  std::size_t m_szBlockSize;  
  std::size_t m_szBlockDepth;
  Counters m_counters;
  CommandBlock m_CommandBlock;  
  std::map<std::string, std::pair<std::size_t, CommandBlock> > m_CommandBlocks;
};

} // otus::
