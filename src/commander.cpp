#include "commander.h"
#include "counter.h"

#include <string>

namespace otus {

Commander::Commander(const std::string& a_strName, std::size_t a_szBlockSize, std::ostream& a_osMetricsOut) 
  : m_osMetricsOut{a_osMetricsOut}
  , m_szBlockSize{a_szBlockSize}
  , m_szBlockDepth{0}
  , m_counters{a_strName}
  , m_CommandBlock{a_szBlockSize}
{ }

Commander::~Commander()
{
  m_osMetricsOut << m_counters << std::endl; 
}

std::shared_ptr<Commander> Commander::Create(const std::string& a_strName, std::size_t a_szBlockSize, std::ostream& a_osMetricsOut)
{
  auto ptr = std::shared_ptr<Commander>{ new Commander{a_strName, a_szBlockSize, a_osMetricsOut} };
  return ptr;
}

void Commander::ProccessLine(const std::string& a_strLine)
{
  if (a_strLine.empty()) {
    return;
  }

  ++m_counters.lineCounter;
  if (a_strLine == "{") {
    if (m_szBlockDepth == 0) {
      Flush();
    }
    m_szBlockDepth++;
  } 
  else if (a_strLine == "}") {
    if (m_szBlockDepth == 0) {
      m_CommandBlock.Clear();
    }
    m_szBlockDepth--;
  }
  else {
    m_CommandBlock << a_strLine;
  }

  if (!m_szBlockDepth && m_CommandBlock.Size() >= m_szBlockSize) {
    Flush();
  }
}

void Commander::Flush() 
{
  if (m_CommandBlock.Size() > 0) {
    Notify(m_CommandBlock);    
    m_counters.commandCounter += m_CommandBlock.Size();
    ++m_counters.blockCounter;
    m_CommandBlock.Clear();
  }
}

} // otus::
