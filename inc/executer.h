#pragma once

#include "command.h"
#include "observer.h"
#include "commander.h"

#include <memory>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>

namespace otus {

class Executer : public IObserver<CommandBlock>, public std::enable_shared_from_this<Executer>
{
public:
  static std::shared_ptr<Executer> Create(const std::string& a_strName, std::shared_ptr<Commander>& a_pCommander, std::ostream& a_osOut = std::cout, std::ostream& a_osMetricsOut = std::cout);
  ~Executer();

  void Update(const CommandBlock& a_CommandBlock) override;

private:
  Executer(const std::string& a_strName, std::ostream& a_osOut, std::ostream& a_osMetricsOut);
  void SetCommander(std::shared_ptr<Commander>& a_pCommander);
  
  void Procces(std::string a_strName);
  void JoinThred();
  
private:
  std::ostream& m_osOut;
  std::ostream& m_osMetricsOut;
  std::weak_ptr<Commander> m_pCommander;

  std::atomic<bool> m_bDone;
  std::thread m_thread;
  std::mutex m_queueLock;
  std::mutex m_printLock;
  std::condition_variable m_queueCheck;
  QueueCommandBllock_t m_queueCommand;
};

} // otus::
