#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <iostream>
#include <fstream>
#include "logger.h"

class FileOutput : public logger::Output
{
private:
  std::ofstream *output;

public:
  FileOutput(std::ofstream *output)
  {
    this->output = output;
  }

  void print(const char *msg)
  {
    *output << msg;
  }
  void print(char c)
  {
    *output << c;
  }
  void print(int v, int b)
  {
    *output << v;
  }
};

class TestProgressLogger : public ::testing::EmptyTestEventListener
{
  virtual void OnTestStart(const ::testing::TestInfo &test_info)
  {
    logger_log("*** Test %s.%s starting.",
               test_info.test_suite_name(), test_info.name());
  }

  virtual void OnTestPartResult(const ::testing::TestPartResult &test_part_result)
  {
    // const char * status = test_part_result.failed() ? "*** Failure" : "Success";
    // auto file_name = test_part_result.file_name();
    // if(file_name == nullptr)
    // {
    //   file_name = "unknown file";
    // }
    // logger_log("%s in %s:%d\n%s\n",
    //            status,
    //            file_name,
    //            test_part_result.line_number(),
    //            test_part_result.summary());
  }

  virtual void OnTestEnd(const ::testing::TestInfo &test_info)
  {
    const testing::TestResult * result = test_info.result();

    const char * status = result->Failed() ? "*** Failure" : "Success";
    logger_log("*** Test %s.%s end with: %s",
               test_info.test_suite_name(), 
               test_info.name(),
               status
               );
  }
};

int main(int argc, char **argv)
{
  std::srand(std::time(nullptr));
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::InitGoogleMock(&argc, argv);
  std::ofstream testLogOutput("test.log", std::ofstream::out | std::ofstream::trunc);
  logger::setupLoggingSystem(new FileOutput(&testLogOutput));

  ::testing::TestEventListeners &listeners = ::testing::UnitTest::GetInstance()->listeners();
  listeners.Append(new TestProgressLogger);

  return RUN_ALL_TESTS();
}