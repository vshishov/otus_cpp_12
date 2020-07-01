#pragma once

#include "observer.h"
#include "command.h"
#include <counter.h>

#include <iostream>
#include <string>
#include <map>
#include <set>

namespace otus {

struct ClientCommand {
  std::size_t m_szNestedLevel;
  CommandBlock m_CommandBlock;  
};

using Context = std::map<std::string, ClientCommand>;
using Context_iter = std::map<std::string, ClientCommand>::iterator;

class Commander : public BaseObservable<CommandBlock>
{
public:
  static std::shared_ptr<Commander> Create(const std::string& a_strName, std::size_t a_szBlockSize, std::ostream& a_osMetricsOut = std::cout);
  ~Commander();

  void ProcessLine(const std::string& a_strId, const std::string& a_strLine);

  void Register(const std::string& a_strClientId);
  void Unregister(const std::string& a_strClientId);

private:
  Commander(const std::string& a_strName, std::size_t a_szBlockSize, std::ostream& a_osMetricsOut = std::cout);

  void Flush(bool a_bForce = false);

  Context_iter GetCurrentContext(const std::string& a_strId);
  Context_iter AddContext(const std::string& a_strId);

private:
  std::ostream& m_osMetricsOut;
  std::size_t m_szBlockSize;  
  std::size_t m_szBlockDepth;
  Counters m_counters;
  Context m_ClientContext;
  std::set<std::string> m_Clients;
};

} // otus::
