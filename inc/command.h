#pragma once

#include <chrono>
#include <string>
#include <vector>
#include <iostream>
#include <queue>

namespace otus {

class CommandBlock
{
public:
  CommandBlock() = default;
  
  CommandBlock(size_t a_nBlockSize)
  { 
    m_vCommands.reserve(a_nBlockSize);
  }

  std::string GetTimeStamp() const 
  {
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(
      m_TimeStamp.time_since_epoch()).count();
    
    return std::to_string(seconds);
  }

  bool Empty() const
  {
    return m_vCommands.empty();
  }

  std::size_t Size() const
  {
    return m_vCommands.size();
  }

  void Clear()
  {
    m_vCommands.clear();
  }

  CommandBlock& operator <<(const std::string& a_strCommand)
  {
    if (m_vCommands.empty()) {
      m_TimeStamp = std::chrono::system_clock::now();
    }

    if (!a_strCommand.empty()) {
      m_vCommands.emplace_back(a_strCommand);
    }

    return *this;
  }

  friend std::ostream& operator<<(std::ostream& a_osOut, const CommandBlock& a_CommandBlock)
  {
    for (auto command_it = a_CommandBlock.m_vCommands.begin(); command_it != a_CommandBlock.m_vCommands.cend(); ++command_it) {
      if (command_it != a_CommandBlock.m_vCommands.begin()) {
        a_osOut << ", ";
      }
      a_osOut << *command_it;
    }
    return a_osOut;
  }

private:
  std::chrono::system_clock::time_point m_TimeStamp;
  std::vector<std::string> m_vCommands;
};

using QueueCommandBllock_t = std::queue<CommandBlock>;

} // otus::
