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

void Commander::ProcessLine(const std::string& a_strId, const std::string& a_strLine)
{
  if (a_strLine.empty()) {
    return;
  }
  
  auto& curBlock = m_CommandBlocks["main"];
  std::cout << a_strId  << std::endl;
  auto curId_iter = m_CommandBlocks.find(a_strId);
  if ( curId_iter != m_CommandBlocks.end() ) {
    std::cout << "find command block" << std::endl;
    curBlock = curId_iter->second;
  }

  ++m_counters.lineCounter;
  if (a_strLine == "{") {
    curBlock = m_CommandBlocks[a_strId];
    if (m_szBlockDepth == 0) {
      Flush();      
    }
    m_szBlockDepth++;
    curBlock.first++;
  } 
  else if (a_strLine == "}") {
    if (curBlock.first > 0) {
      curBlock.first--;
    }
    else {      
      curBlock.second.Clear();
    }
  }
  else {
    // m_CommandBlock << a_strLine;  
    std::cout << "add: " << m_CommandBlocks.size() << std::endl;

    curBlock.second << a_strLine;
    std::cout << "after add: " << m_CommandBlocks.size() << std::endl;
  }

 std::cout << "size: " << curBlock.second.Size() << std::endl;
 std::cout << "curNested: " << curBlock.first << std::endl;
  if (!curBlock.first && curBlock.second.Size() >= m_szBlockSize) {
    // Flush();
    std::cout << "Flush()" << std::endl;

    if (curBlock.second.Size() > 0) {
      Notify(curBlock.second);    
      m_counters.commandCounter += curBlock.second.Size();
      ++m_counters.blockCounter;
      curBlock.second.Clear();
      // m_CommandBlocks.erase(curBlock);
    }
  }
}

void Commander::Stop(const std::string& a_strId)
{
  std::cout << a_strId;
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
