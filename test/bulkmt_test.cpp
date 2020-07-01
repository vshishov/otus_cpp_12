#include "commander.h"
#include "executer.h"

#include <gtest/gtest.h>
#include <sstream>
#include <iostream>
#include <thread>
#include <chrono>

using namespace testing;

class Bulkmt_Test : public ::testing::Test
{
protected:
  void Run(std::size_t a_nBlockSize, std::size_t a_szDelay = 1)
  {      
    auto pCommander = otus::Commander::Create("main", a_nBlockSize, ssMetricsMain);
    auto pExecuter = otus::Executer::Create("log", pCommander, ssTestOut, ssMetricsLog);
   
    std::string strIdentifier {"test"};

    pCommander->Register(strIdentifier);
    std::string strLine;
    while ( std::getline(ssTestIn, strLine) ) {
      pCommander->ProcessLine(strIdentifier, strLine);
    }    

    std::this_thread::sleep_for(std::chrono::seconds(a_szDelay));
    pCommander->Unregister(strIdentifier);
  }

protected:
  std::stringstream ssTestIn;
  std::stringstream ssTestOut;
  std::stringstream ssMetricsMain;
  std::stringstream ssMetricsLog;
};

TEST_F(Bulkmt_Test, empty_stream) {   
  Run(2);

  ASSERT_EQ(ssTestOut.str(), "");
}

TEST_F(Bulkmt_Test, not_empty_stream) { 
  ssTestIn << "cmd1" << std::endl
    << "cmd2" << std::endl
    << "cmd3" << std::endl
  ;

  Run(3);

  ASSERT_EQ(ssTestOut.str(), "bulk: cmd1, cmd2, cmd3\n");
  ASSERT_EQ(ssMetricsMain.str(), "main: 3 line(s), 1 block(s), 3 command(s)\n");
  ASSERT_EQ(ssMetricsLog.str(), "log: 1 block(s), 3 command(s)\n");
}

TEST_F(Bulkmt_Test, not_complete) { 
  ssTestIn << "cmd1" << std::endl
      << "cmd2" << std::endl
    ;

  {
    Run(3);
  }

  ASSERT_EQ(ssTestOut.str(), "bulk: cmd1, cmd2\n");
  ASSERT_EQ(ssMetricsMain.str(), "main: 2 line(s), 1 block(s), 2 command(s)\n");
  ASSERT_EQ(ssMetricsLog.str(), "log: 1 block(s), 2 command(s)\n");
}

TEST_F(Bulkmt_Test, single_block) { 
  ssTestIn << "{" << std::endl
    << "cmd1" << std::endl
    << "cmd2" << std::endl
    << "cmd3" << std::endl
    << "cmd4" << std::endl
    << "cmd5" << std::endl
    << "}" << std::endl
  ;

  Run(3);
  
  ASSERT_EQ(ssTestOut.str(), "bulk: cmd1, cmd2, cmd3, cmd4, cmd5\n");
  ASSERT_EQ(ssMetricsMain.str(), "main: 7 line(s), 1 block(s), 5 command(s)\n");
  ASSERT_EQ(ssMetricsLog.str(), "log: 1 block(s), 5 command(s)\n");
}

TEST_F(Bulkmt_Test, single_block_in_default_block) { 
  ssTestIn << "cmd1" << std::endl
    << "cmd2" << std::endl
    << "{" << std::endl
    << "cmd3" << std::endl
    << "cmd4" << std::endl
    << "cmd5" << std::endl
    << "cmd6" << std::endl
    << "cmd7" << std::endl
    << "}" << std::endl
    << "cmd8" << std::endl
  ;

  Run(3, 2);

  ASSERT_EQ(ssTestOut.str(), "bulk: cmd3, cmd4, cmd5, cmd6, cmd7\nbulk: cmd1, cmd2, cmd8\n");
  ASSERT_EQ(ssMetricsMain.str(), "main: 10 line(s), 2 block(s), 8 command(s)\n");
  ASSERT_EQ(ssMetricsLog.str(), "log: 2 block(s), 8 command(s)\n");
}

TEST_F(Bulkmt_Test, multi_block) { 
  ssTestIn << "cmd1" << std::endl
    << "cmd2" << std::endl
    << "{" << std::endl
    << "cmd1.1" << std::endl
    << "cmd1.2" << std::endl
    << "cmd1.3" << std::endl
    << "{" << std::endl
    << "cmd2.1" << std::endl
    << "cmd2.2" << std::endl
    << "cmd2.3" << std::endl
    << "cmd2.4" << std::endl
    << "cmd2.5" << std::endl
    << "}" << std::endl
    << "cmd1.4" << std::endl
    << "cmd1.5" << std::endl
    << "}" << std::endl
    << "cmd3" << std::endl;
  ;

  Run(3);

  ASSERT_EQ(ssTestOut.str(), "bulk: cmd1.1, cmd1.2, cmd1.3, "
    "cmd2.1, cmd2.2, cmd2.3, cmd2.4, cmd2.5, "
    "cmd1.4, cmd1.5\n"
    "bulk: cmd1, cmd2, cmd3\n");
  ASSERT_EQ(ssMetricsMain.str(), "main: 17 line(s), 2 block(s), 13 command(s)\n");
  ASSERT_EQ(ssMetricsLog.str(), "log: 2 block(s), 13 command(s)\n");
}

TEST_F(Bulkmt_Test, blocks_not_complete) { 
  ssTestIn << "cmd1" << std::endl
    << "cmd2" << std::endl
    << "{" << std::endl
    << "cmd1.1" << std::endl
    << "cmd1.2" << std::endl
    << "cmd1.3" << std::endl
    << "{" << std::endl
    << "cmd2.1" << std::endl
    << "cmd2.2" << std::endl
    << "cmd2.3" << std::endl
    << "cmd2.4" << std::endl
    << "cmd2.5" << std::endl
    << "}" << std::endl
    << "cmd1.4" << std::endl
    << "cmd1.5" << std::endl
  ;

  Run(3);

  ASSERT_EQ(ssTestOut.str(), "bulk: cmd1, cmd2\n");
  ASSERT_EQ(ssMetricsMain.str(), "main: 15 line(s), 1 block(s), 2 command(s)\n");
  ASSERT_EQ(ssMetricsLog.str(), "log: 1 block(s), 2 command(s)\n");
}
