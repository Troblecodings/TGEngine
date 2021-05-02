#include <TGEngine.hpp>
#include <gtest/gtest.h>
#include <thread>

bool hasInit = false;
bool hasTick = false;
bool hasDestroy = false;

using namespace tge::main;

class TestModule : public tge::main::Module {
  Error init() {
    hasInit = true;
    return Error::NONE;
  }
  void tick(double time) { hasTick = true; }
  void destroy() { hasDestroy = true; }
};

int exitCode = 0;
bool finishedInit = false;
Error err;
bool finishedExit = false;
bool finishedTest = false;

int main() {
  testing::InitGoogleTest();
  auto thr = std::thread([rt = &exitCode, rt2 = &finishedTest]() {
    *rt = RUN_ALL_TESTS();
    *rt2 = true;
  });
  thr.detach();
  while (!finishedInit)
    continue;
  err = start();
  finishedExit = true;
  while (!finishedTest)
    continue;
  return exitCode;
}

TEST(EngineMain, Start) {
  tge::main::modules.push_back(new TestModule());
  ASSERT_EQ(start(), Error::NOT_INITIALIZED);
  ASSERT_EQ(init(), Error::NONE);
  ASSERT_EQ(init(), Error::ALREADY_INITIALIZED);
  finishedInit = true;
}

TEST(EngineMain, InitAndTickTest) {
  ASSERT_EQ(start(), Error::ALREADY_RUNNING);
  ASSERT_TRUE(hasInit);
  ASSERT_TRUE(hasTick);
}

TEST(EngineMain, Exit) {
  ASSERT_NO_FATAL_FAILURE(tge::main::requestExit());
  while (!finishedExit)
    continue;
  ASSERT_TRUE(hasDestroy);
  ASSERT_EQ(err, Error::NONE);
}
