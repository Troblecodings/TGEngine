#include <TGEngine.hpp>
#include <graphics/GameGraphicsModule.hpp>
#include <graphics/VulkanGraphicsModule.hpp>
#include <gtest/gtest.h>
#include <thread>

#if defined(_MSC_VER) and not defined(__clang__)
#pragma optimize("", off)
#endif // MSVC

bool hasInit = false;
bool hasTick = false;
bool hasDestroy = false;

using namespace tge::main;
using namespace tge::graphics;

double alltime = 0;

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
bool finishedInit = false;
Error err;
bool finishedExit = false;
bool finishedTest = false;

#ifdef __clang__
__attribute__((optnone))
#endif // __clang__
int main() {
  auto thr = std::thread([rt = &exitCode, rt2 = &finishedTest]() {
    testing::InitGoogleTest();
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

TEST(Shader, LoadAndCompile) {
  Material mat;
  std::vector<std::string> test = {"test.vert", "test.frag"};
  ASSERT_NO_THROW(mat.costumShaderData = loadShaderPipeAndCompile(test));
  ASSERT_NE(mat.costumShaderData, nullptr);
  materials.push_back(mat);
}

TEST(EngineMain, Start) {
  tge::main::modules.push_back(new TestModule());
  ASSERT_EQ(start(), Error::NOT_INITIALIZED);
  ASSERT_EQ(init(), Error::NONE);
  APILayer *apiLayer = getAPILayer();
  ASSERT_EQ(apiLayer->pushMaterials(materials.size(), materials.data()),
            Error::NONE);
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
  ASSERT_EQ(apiLayer->pushData(1, (const uint8_t **)dptr.data(), size.data(),
                               DataType::VertexData),
            Error::NONE);

  const std::array indexData = {0, 1, 2, 2, 3, 0};
  const std::array indexdptr = {indexData.data()};
  const std::array indexsize = {indexData.size() * sizeof(int)};
  ASSERT_EQ(apiLayer->pushData(1, (const uint8_t **)indexdptr.data(),
                               indexsize.data(), DataType::IndexData),
            Error::NONE);

  RenderInfo renderInfo;
  renderInfo.indexBuffer = 1;
  renderInfo.materialId = 0;
  renderInfo.indexCount = indexData.size();
  renderInfo.vertexBuffer.push_back(0);
  ASSERT_EQ(apiLayer->pushRender(1, &renderInfo), Error::NONE);

  ASSERT_EQ(init(), Error::ALREADY_INITIALIZED);
  finishedInit = true;
}

TEST(EngineMain, InitAndTickTest) {
  ASSERT_EQ(start(), Error::ALREADY_RUNNING);
  ASSERT_TRUE(hasInit);
  ASSERT_TRUE(hasTick);
  while (alltime < 5)
    continue;
}

TEST(EngineMain, Exit) {
  alltime = 0;
  while (alltime < 5)
    continue;
  ASSERT_NO_FATAL_FAILURE(tge::main::requestExit());
  while (!finishedExit)
    continue;
  ASSERT_TRUE(hasDestroy);
  ASSERT_EQ(err, Error::NONE);
}
