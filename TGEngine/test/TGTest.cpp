#include <TGEngine.hpp>
#include <Util.hpp>
#include <fstream>
#define GLM_FORCE_DEPTH_ZERO_TO_ONE 1
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <graphics/GameGraphicsModule.hpp>
#include <graphics/vulkan/VulkanGraphicsModule.hpp>
#include <graphics/vulkan/VulkanShaderPipe.hpp>
#include <gtest/gtest.h>
#include <headerlibs/json.hpp>
#include <imgui.h>
#include <mutex>
#include <thread>

#define DEFAULT_TIME (120.0f)

#define MODEL_TEST 0

bool hasInit = false;
bool hasTick = false;
bool hasDestroy = false;

using namespace tge::main;
using namespace tge::graphics;
using namespace tge::util;

void exitWaitCheck();

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

int main(int argv, char **in) {
  auto thr = std::thread([&, rt = &exitCode, exReq = &testExitRequest]() {
    syncMutex.lock();
    testing::InitGoogleTest(&argv, in);
    *rt = RUN_ALL_TESTS();
    syncMutex.unlock();
    *exReq = true;
    requestExit();
  });
  thr.detach();
  while (!testExitRequest) {
    syncMutex.lock();
    err = start();
    syncMutex.unlock();
    printf(" ");
  }
  std::lock_guard guard(syncMutex);
  return exitCode;
}

class TableTest : public tge::main::Module {
public:
  GameGraphicsModule *ggm;
  float rotation = 0;
  size_t nodeID;
  glm::vec3 scale = glm::vec3(4, -4, 4);
  Light light = Light({10, 5, 0}, {1, 1, 1}, 0.5f);
  bool rotate = true;

  void tick(double time) {
    if (rotate) {
      rotation += (float)time;
      const NodeTransform transform = {
          {}, scale, glm::toQuat(glm::rotate(rotation, glm::vec3(0, 1, 0)))};
      ggm->updateTransform(nodeID, transform);
    }
    ggm->getAPILayer()->pushLights(1, &light);
  }
};

float *__tmp;
float *__r;
bool *__rotate;

TEST(EngineMain, TestModel) {
  tge::main::lateModules.push_back(new TestModule());
  TableTest *av = new TableTest();
  tge::main::lateModules.push_back(av);

  ASSERT_EQ(init(), Error::NONE);
  av->ggm = getGameGraphicsModule();
  
  __tmp = &av->light.intensity;
  __r = &av->light.pos.x;
  __rotate = &av->rotate;

  const auto vec = tge::util::wholeFile("assets/Test/test.gltf");
  const auto mdlID =
      getGameGraphicsModule()->loadModel(vec, false, "assets/Test/");
  ASSERT_NE(mdlID, UINT64_MAX);
  av->nodeID = mdlID;
  av->scale = glm::vec3(0.02f, 0.02f, 0.02f);

  syncMutex.unlock();
  waitForTime();
  exitWaitCheck();
}

void defaultTestData() {
  APILayer *apiLayer = getAPILayer();
  size_t materialOffset;

  std::vector<std::string> test = {"assets/testvec4.vert", "assets/test.frag"};
  Material mat;
  ASSERT_NO_THROW(
      mat.costumShaderData =
          getAPILayer()->getShaderAPI()->loadShaderPipeAndCompile(test));
  ASSERT_NE(mat.costumShaderData, nullptr);

  ASSERT_NO_THROW(materialOffset = apiLayer->pushMaterials(1, &mat));
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
  size_t __dNoDiscard1;
  ASSERT_NO_THROW(__dNoDiscard1 = apiLayer->pushData(
                      1, (const uint8_t **)indexdptr.data(), indexsize.data(),
                      DataType::IndexData));

  RenderInfo renderInfo;
  renderInfo.indexBuffer = 1 + dataOffset;
  renderInfo.materialId = materialOffset;
  renderInfo.indexCount = indexData.size();
  renderInfo.vertexBuffer.push_back(dataOffset);
  ASSERT_NO_THROW(apiLayer->pushRender(1, &renderInfo));
}

class Avocado2Test : public tge::main::Module {
public:
  GameGraphicsModule *ggm;
  float rotation = 0;
  size_t nodeID;
  glm::vec3 scale = glm::vec3(4, -4, 4);

  void tick(double time) {
    rotation += (float)time;
    const NodeTransform transform = {
        {}, scale, glm::toQuat(glm::rotate(rotation, glm::vec3(0, 1, 0)))};
    ggm->updateTransform(nodeID, transform);
  }
};

TEST(EngineMain, AvocadoTestOne) {
  tge::main::lateModules.push_back(new TestModule());
  Avocado2Test *av = new Avocado2Test();
  tge::main::lateModules.push_back(av);

  ASSERT_EQ(init(), Error::NONE);
  av->ggm = getGameGraphicsModule();

  std::vector<std::string> test = {"assets/avocado.vert",
                                   "assets/testTexture.frag"};
  auto ptr = (tge::shader::VulkanShaderPipe *)getAPILayer()
                 ->getShaderAPI()
                 ->loadShaderPipeAndCompile(test);

  const auto vec = tge::util::wholeFile(
      "assets/glTF-Sample-Models/2.0/Avocado/glTF/Avocado.gltf");
  const auto mdlID = getGameGraphicsModule()->loadModel(
      vec, false, "assets/glTF-Sample-Models/2.0/Avocado/glTF/", ptr);
  ASSERT_NE(mdlID, UINT64_MAX);
  av->nodeID = mdlID;

  syncMutex.unlock();
  waitForTime();
  exitWaitCheck();
}

TEST(ShaderCompiler, Create) {
  tge::main::lateModules.push_back(new TestModule());

  ASSERT_EQ(init(), Error::NONE);

  tge::shader::ShaderCreateInfo sh[2];
  sh[0].__code = "gl_Position = ublock_0.mvp * test;";
  sh[0].inputs.push_back({"test", tge::shader::IOType::VEC4, 0});
  sh[0].outputs.push_back({"testout", tge::shader::IOType::VEC2, 0});
  sh[0].unifromIO.push_back({"mvp", tge::shader::IOType::MAT4, 2});
  sh[0].shaderType = tge::shader::ShaderType::VERTEX;

  sh[1].__code = "color = texture(sampler2D(diffuse, samp), testin);";
  sh[1].outputs.push_back({"color", tge::shader::IOType::VEC4, 0});
  sh[1].inputs.push_back({"testin", tge::shader::IOType::VEC2, 1});
  sh[1].samplerIO.push_back({"samp", tge::shader::SamplerIOType::SAMPLER, 0});
  sh[1].samplerIO.push_back(
      {"diffuse", tge::shader::SamplerIOType::TEXTURE, 1});
  sh[1].shaderType = tge::shader::ShaderType::FRAGMENT;

  void *__noDiscard;
  ASSERT_NO_THROW(__noDiscard =
                      getAPILayer()->getShaderAPI()->createShaderPipe(sh, 2));

  namespace s = tge::shader;
  sh[0].__code = "";
  sh[0].instructions = {
      {{"ublock_0.mvp", "test"},
       s::IOType::VEC4,
       s::InstructionType::MULTIPLY,
       "_tmp1"},
      {{"_tmp1"}, s::IOType::VEC4, s::InstructionType::SET, "gl_Position"}
  };

  sh[1].__code = "";
  sh[1].instructions = {
      {{"sampler2D(diffuse, samp)", "testin"}, s::IOType::VEC4, s::InstructionType::TEXTURE, "_tmp"},
      {{"_tmp"}, s::IOType::VEC4, s::InstructionType::SET, "color"}
  };
  ASSERT_NO_THROW(__noDiscard =
                      getAPILayer()->getShaderAPI()->createShaderPipe(sh, 2));

  syncMutex.unlock();
  waitForTime();
  exitWaitCheck();
}

TEST(EngineMain, SamplerAndTextures) {
  tge::main::lateModules.push_back(new TestModule());

  ASSERT_EQ(init(), Error::NONE);

  const SamplerInfo sampler = {FilterSetting::NEAREST, FilterSetting::NEAREST,
                               AddressMode::REPEAT, AddressMode::REPEAT, 0};
  APILayer *apiLayer = getAPILayer();

  TextureMaterial texMat;

  ASSERT_NO_THROW(texMat.samplerIndex = apiLayer->pushSampler(sampler));

  size_t __dNoDiscard1 = 0;
  ASSERT_ANY_THROW(__dNoDiscard1 = getGameGraphicsModule()->loadTextures(
                       {"assets/test3c.png"}));

  ASSERT_NO_THROW(texMat.textureIndex = getGameGraphicsModule()->loadTextures(
                      {"assets/test.png"}));

  const Material mat(texMat, apiLayer);

  size_t materialOffset;
  ASSERT_NO_THROW(materialOffset = apiLayer->pushMaterials(1, &mat));
  const std::array vertData = {
      0.0f, 0.0f, -1.0f, 0.0f, 0.2f, 1.0f, //
      1.0f, 0.0f, 1.0f,  0.0f, 0.2f, 1.0f, //
      1.0f, 1.0f, 1.0f,  1.0f, 0.2f, 1.0f, //
      0.0f, 1.0f, -1.0f, 1.0f, 0.2f, 1.0f, //
  };

  const std::array dptr = {vertData.data()};
  const std::array size = {vertData.size() * sizeof(float)};
  size_t dataOffset = -1;
  ASSERT_NO_THROW(dataOffset =
                      apiLayer->pushData(1, (const uint8_t **)dptr.data(),
                                         size.data(), DataType::VertexData));

  const std::array indexData = {0, 1, 2, 2, 3, 0};
  const std::array indexdptr = {indexData.data()};
  const std::array indexsize = {indexData.size() * sizeof(int)};
  size_t indexBuffer = -1;
  ASSERT_NO_THROW(
      indexBuffer = apiLayer->pushData(1, (const uint8_t **)indexdptr.data(),
                                       indexsize.data(), DataType::IndexData));

  RenderInfo renderInfo;
  renderInfo.indexBuffer = indexBuffer;
  renderInfo.materialId = materialOffset;
  renderInfo.indexCount = indexData.size();
  renderInfo.vertexBuffer.push_back(dataOffset);
  ASSERT_NO_THROW(apiLayer->pushRender(1, &renderInfo));

  syncMutex.unlock();
  waitForTime();
  exitWaitCheck();
}

TEST(EngineMain, Start) {
  tge::main::lateModules.push_back(new TestModule());
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
  ASSERT_NO_FATAL_FAILURE(requestExit());
  syncMutex.lock();
  ASSERT_TRUE(hasDestroy);
  ASSERT_EQ(err, Error::NONE);
}

TEST(EngineMain, Exit) { exitWaitCheck(); }

TEST(EngineMain, Restart) { ASSERT_EQ(lateModules.size(), 0); }

TEST(EngineMain, SimpleModel) {
  tge::main::lateModules.push_back(new TestModule());

  ASSERT_EQ(init(), Error::NONE);

  const auto data = tge::util::wholeFile("assets/Triangle.gltf");
  ASSERT_FALSE(data.empty());
  ASSERT_NE(getGameGraphicsModule()->loadModel(data, false, "assets"),
            UINT64_MAX);

  syncMutex.unlock();
  waitForTime();
  exitWaitCheck();
}

TEST(EngineApi, GameAPIChecks) {}

TEST(EngineApi, GraphicsAPIChecks) {
  APILayer *apiLayer = getAPILayer();
  ASSERT_EQ(apiLayer, nullptr);
  ASSERT_NO_THROW(apiLayer = getNewVulkanModule());
  ASSERT_NE(apiLayer, nullptr);
  const uint8_t data = 1;
  const uint8_t *dataptr = &data;
  const size_t size = 1;
  size_t __dNoDiscard1;
  ASSERT_THROW(__dNoDiscard1 =
                   apiLayer->pushData(1, &dataptr, &size, DataType::VertexData),
               std::runtime_error);
  tge::graphics::Material mat;
  ASSERT_THROW(__dNoDiscard1 = apiLayer->pushMaterials(1, &mat),
               std::runtime_error);

  tge::graphics::RenderInfo renderInfo;
  ASSERT_THROW(apiLayer->pushRender(1, &renderInfo), std::runtime_error);

  tge::graphics::TextureInfo textureInfo;
  ASSERT_THROW(__dNoDiscard1 = apiLayer->pushTexture(1, &textureInfo),
               std::runtime_error);

  void *__dNoDiscard2;
  for (size_t i = 0; i <= (size_t)MAX_TYPE; i++) {
    ASSERT_THROW(__dNoDiscard2 = apiLayer->loadShader((MaterialType)i),
                 std::runtime_error);
  }

  delete apiLayer;

  ASSERT_EQ(init(), Error::NONE);
  apiLayer = getAPILayer();
  ASSERT_NE(apiLayer, nullptr);

  for (size_t i = 0; i <= (size_t)MAX_TYPE; i++) {
    ASSERT_NO_THROW(__dNoDiscard2 = apiLayer->loadShader((MaterialType)i));
  }

  ASSERT_THROW(__dNoDiscard2 =
                   apiLayer->loadShader((MaterialType)10000000000000),
               std::runtime_error);

  ASSERT_THROW(__dNoDiscard1 =
                   apiLayer->pushData(0, &dataptr, &size, DataType::VertexData),
               std::runtime_error);
  ASSERT_THROW(__dNoDiscard1 = apiLayer->pushMaterials(0, &mat),
               std::runtime_error);
  ASSERT_THROW(apiLayer->pushRender(0, &renderInfo), std::runtime_error);
  ASSERT_THROW(__dNoDiscard1 = apiLayer->pushTexture(0, &textureInfo),
               std::runtime_error);

  ASSERT_THROW(__dNoDiscard1 =
                   apiLayer->pushData(1, nullptr, &size, DataType::VertexData),
               std::runtime_error);
  ASSERT_THROW(__dNoDiscard1 = apiLayer->pushData(1, &dataptr, nullptr,
                                                  DataType::VertexData),
               std::runtime_error);
  ASSERT_THROW(__dNoDiscard1 = apiLayer->pushMaterials(1, nullptr),
               std::runtime_error);
  ASSERT_THROW(apiLayer->pushRender(1, nullptr), std::runtime_error);
  ASSERT_THROW(__dNoDiscard1 = apiLayer->pushTexture(1, nullptr),
               std::runtime_error);
}

TEST(EngineApi, Exit) {
  syncMutex.unlock();
  exitWaitCheck();
}

#if MODEL_TEST
TEST(EngineModel, ModelTest) {
  const std::string path = "assets/glTF-Sample-Models/2.0/";
  nlohmann::json js;
  std::ifstream fstr(path + "model-index.json");
  fstr >> js;
  for (const auto &pthObj : js) {
    const auto name = pthObj["name"].get<std::string>();
    std::cout << "==================" << std::endl
              << name << std::endl
              << "==================" << std::endl;
    for (const auto &pair : pthObj["variants"].items()) {
      const auto &typname = pair.key();
      if (typname.compare("glTF-Draco") == 0)
        continue;
      const std::string actualPath = path + name + "/" + typname + "/";
      const std::string actualFile =
          actualPath + pair.value().get<std::string>();

      tge::main::lateModules.push_back(new TestModule());

      ASSERT_EQ(init(), Error::NONE);

      const auto data = tge::util::wholeFile(actualFile);
      ASSERT_FALSE(data.empty());
      ASSERT_EQ(getGameGraphicsModule()->loadModel(
                    data, typname.compare("glTF-Binary") == 0, actualPath),
                Error::NONE);

      syncMutex.unlock();
      waitForTime();
      exitWaitCheck();
    }
  }
}
#endif
