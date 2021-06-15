#include <TGEngine.hpp>
#include <Util.hpp>
#include <graphics/GameGraphicsModule.hpp>
#include <graphics/VulkanGraphicsModule.hpp>
#include <gtest/gtest.h>
#include <mutex>
#include <thread>

#define DEFAULT_TIME 1

bool hasInit = false;
bool hasTick = false;
bool hasDestroy = false;

using namespace tge::main;
using namespace tge::graphics;

double alltime = 0;
std::mutex syncMutex;

inline void waitForTime() {
  alltime = 0;
  while (alltime < DEFAULT_TIME)
    continue;
}

class TestModule : public tge::main::Module {
  Error init() {
    hasInit = true;
    return Error::NONE;
  }
  void tick(double time) {
    hasTick = true;
    alltime += time;
  }
  void destroy() { hasDestroy = true; }
};

int exitCode = 0;
Error err;

bool testExitRequest = false;

int main() {
  auto thr = std::thread([rt = &exitCode]() {
    syncMutex.lock();
    testing::InitGoogleTest();
    *rt = RUN_ALL_TESTS();
    syncMutex.unlock();
  });
  thr.detach();
  while (!testExitRequest) {
    syncMutex.lock();
    err = start();
    syncMutex.unlock();
    printf("TUNLOCK WAIT\n");
  }
  std::lock_guard guard(syncMutex);
  return exitCode;
}

Material mat;

TEST(Shader, LoadAndCompile) {
  std::vector<std::string> test = {"testvec4.vert", "test.frag"};
  ASSERT_NO_THROW(mat.costumShaderData = loadShaderPipeAndCompile(test));
  ASSERT_NE(mat.costumShaderData, nullptr);
}

void defaultTestData() {
  materials.push_back(mat);
  APILayer *apiLayer = getAPILayer();
  size_t materialOffset;
  ASSERT_NO_THROW(materialOffset = apiLayer->pushMaterials(materials.size(),
                                                           materials.data()));
  const std::array vertData = {-1.0f, 0.0f, 0.2f, 1.0f, //
                               1.0f, 0.0f, 0.2f, 1.0f,  //
                               1.0f, 1.0f, 0.2f, 1.0f,  //
                               -1.0f, 1.0f, 0.2f, 1.0f, //
                                                        //
                               -1.0f, 0.0f, 0.1f, 1.0f, //
                               1.0f, 0.0f, 0.1f, 1.0f,  //
                               1.0f, 1.0f, 0.1f, 1.0f,  //
                               -1.0f, 1.0f, 0.1f, 1.0f};
  const std::array dptr = {vertData.data()};
  const std::array size = {vertData.size() * sizeof(float)};
  size_t dataOffset;
  ASSERT_NO_THROW(dataOffset =
                      apiLayer->pushData(1, (const uint8_t **)dptr.data(),
                                         size.data(), DataType::VertexData));

  const std::array indexData = {0, 1, 2, 2, 3, 0};
  const std::array indexdptr = {indexData.data()};
  const std::array indexsize = {indexData.size() * sizeof(int)};
  ASSERT_NO_THROW(apiLayer->pushData(1, (const uint8_t **)indexdptr.data(),
                                     indexsize.data(), DataType::IndexData));

  RenderInfo renderInfo;
  renderInfo.indexBuffer = 1 + dataOffset;
  renderInfo.materialId = materialOffset;
  renderInfo.indexCount = indexData.size();
  renderInfo.vertexBuffer.push_back(dataOffset);
  ASSERT_NO_THROW(apiLayer->pushRender(1, &renderInfo));
}

TEST(EngineMain, Start) {
  tge::main::modules.push_back(new TestModule());
  ASSERT_EQ(start(), Error::NOT_INITIALIZED);
  ASSERT_EQ(init(), Error::NONE);

  defaultTestData();

  ASSERT_EQ(init(), Error::ALREADY_INITIALIZED);
  syncMutex.unlock();
}

TEST(EngineMain, InitAndTickTest) {
  ASSERT_TRUE(hasInit);
  ASSERT_TRUE(hasTick);
  waitForTime();
  ASSERT_EQ(start(), Error::ALREADY_RUNNING);
}

void exitWaitCheck() {
  ASSERT_NO_FATAL_FAILURE(tge::main::requestExit());
  syncMutex.lock();
  ASSERT_TRUE(hasDestroy);
  ASSERT_EQ(err, Error::NONE);
}

TEST(EngineMain, Exit) { exitWaitCheck(); }

TEST(EngineMain, Restart) {
  ASSERT_EQ(materials.size(), 0);
  ASSERT_EQ(modules.size(), 0);
}

TEST(EngineMain, SamplerAndTextures) {
  tge::main::modules.push_back(new TestModule());

  ASSERT_EQ(init(), Error::NONE);

  const SamplerInfo sampler = {FilterSetting::LINEAR, FilterSetting::LINEAR,
                               AddressMode::REPEAT, AddressMode::REPEAT, 0};
  ASSERT_NO_THROW(getAPILayer()->pushSampler(sampler));

  ASSERT_THROW(getGameGraphicsModule()->loadTextures({"test3c.png"}), std::runtime_error);

  ASSERT_NO_THROW(getGameGraphicsModule()->loadTextures({"test.png"}));

  defaultTestData();
  syncMutex.unlock();
  waitForTime();
  exitWaitCheck();
}

TEST(EngineMain, SimpleModel) {
  tge::main::modules.push_back(new TestModule());

  ASSERT_EQ(init(), Error::NONE);

  const auto data = tge::util::wholeFile("Triangle.gltf");
  ASSERT_FALSE(data.empty());
  ASSERT_EQ(getGameGraphicsModule()->loadModel(data, false), Error::NONE);

  syncMutex.unlock();
  waitForTime();
  testExitRequest = true;
  exitWaitCheck();
}
