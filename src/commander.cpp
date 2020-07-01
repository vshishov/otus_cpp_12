#include "commander.h"
#include "counter.h"

#include <string>

namespace otus {

Commander::Commander(const std::string& a_strName, std::size_t a_szBlockSize, std::ostream& a_osMetricsOut) 
  : m_osMetricsOut{a_osMetricsOut}
  , m_szBlockSize{a_szBlockSize}
  , m_szBlockDepth{0}
  , m_counters{a_strName}
  // , m_CommandBlock{a_szBlockSize}
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

  // std::cout << std::endl << "------------" << std::endl;
  //auto& curBlock = m_ClientContext["main"];
  // std::cout << a_strId  << std::endl;
  auto curContext = GetCurrentContext(a_strId);
  // std::cout << "CurrentBlock: " << curContext->first << std::endl;
//   if ( curContext != m_ClientContext.end() ) {
//     std::cout << "find command block" << std::endl;
//     curBlock = curContext->second;
//     std::cout << "cur size: " << curBlock.m_CommandBlock.Size() << std::endl;
//  std::cout << "curNested: " << curBlock.m_szNestedLevel << std::endl;
//  std::cout << "------------" << std::endl << std::endl;
 
//   }

  ++m_counters.lineCounter;
  if (a_strLine == "{") {
    curContext = AddContext(a_strId);
    curContext->second.m_szNestedLevel++;
  } 
  else if (a_strLine == "}") {
    if (curContext->second.m_szNestedLevel > 0) {
      curContext->second.m_szNestedLevel--;
    }
    else {      
      curContext->second.m_CommandBlock.Clear();
    }
  }
  else {
    // m_CommandBlock << a_strLine;  
    // std::cout << "add command: " << m_ClientContext.size() << std::endl;

    curContext->second.m_CommandBlock << a_strLine;
    // std::cout << "after add: " << m_ClientContext.size() << std::endl;
  }

  // std::cout << "size: " << m_ClientContext.size() << std::endl;
  // std::cout << "cur size: " << curContext->second.m_CommandBlock.Size() << std::endl;
  // std::cout << "curNested: " << curContext->second.m_szNestedLevel << std::endl;
  // if (!curContext->second.m_szNestedLevel && curContext->second.m_CommandBlock.Size() >= m_szBlockSize) {
    // std::cout << "Flush()" << std::endl;
    // Flush(curContext);
    // RemoveContext(a_strId);
  // }
  Flush();
}

void Commander::Register(const std::string& a_strClientId)
{
  m_Clients.emplace(a_strClientId);
}

void Commander::Unregister(const std::string& a_strClientId)
{
  m_Clients.erase(a_strClientId);
  if (m_Clients.size() == 0) {
    Flush(true);
  }
}

void Commander::Flush(bool a_bForce) 
{
  auto context_iter = m_ClientContext.begin();
  while ( context_iter != m_ClientContext.end() ) {
  // for ( auto& context_iter : m_ClientContext ) {
    auto& context = context_iter->second; 
    if (!context.m_szNestedLevel && (a_bForce || context.m_CommandBlock.Size() >= m_szBlockSize) ) {
      if (context.m_CommandBlock.Size() > 0) {
        Notify(context.m_CommandBlock);    
        m_counters.commandCounter += context.m_CommandBlock.Size();
        ++m_counters.blockCounter;
        context.m_CommandBlock.Clear();        
      }
      context_iter = m_ClientContext.erase(context_iter);
    }
    else {
      ++context_iter;
    }
  }
}

Context_iter Commander::GetCurrentContext(const std::string& a_strId)
{
  Context_iter iter = m_ClientContext.find(a_strId);
  if (iter == m_ClientContext.end()) {
    iter = AddContext("main");
  }
  return iter;
}

Context_iter Commander::AddContext(const std::string& a_strId)
{
  Context_iter iter = m_ClientContext.find(a_strId);
  if (iter == m_ClientContext.end()) {
    auto [inserted_iter, inserted] = m_ClientContext.try_emplace(a_strId);
    if (inserted) {
      iter = inserted_iter;
    }
  }
  return iter;
}

void Commander::RemoveContext(const std::string& a_strId)
{
  m_ClientContext.erase(a_strId);
}

} // otus::
