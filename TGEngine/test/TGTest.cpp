#include <TGEngine.hpp>
#include <Util.hpp>
#include <fstream>
#define GLM_FORCE_DEPTH_ZERO_TO_ONE 1
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <graphics/GameGraphicsModule.hpp>
#include <graphics/VulkanGraphicsModule.hpp>
#include <graphics/VulkanShaderPipe.hpp>
#include <gtest/gtest.h>
#include <headerlibs/json.hpp>
#include <mutex>
#include <thread>

#define DEFAULT_TIME (10.0f)

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

  void tick(double time) {
    rotation += (float)time;
    const NodeTransform transform = {
        {},
        glm::vec3(4, 4, 4),
        glm::toQuat(glm::rotate(rotation, glm::vec3(0, 1, 0)))};
    ggm->updateTransform(nodeID, transform);
  }
};

TEST(ShaderCompiler, Create) {
  tge::main::modules.push_back(new TestModule());

  ASSERT_EQ(init(), Error::NONE);

  tge::shader::ShaderCreateInfo sh;
  std::string str("out gl_PerVertex { vec4 gl_Position; }; void main() { }");
  sh.code = std::vector<char>(str.begin(), str.end());
  sh.inputs.push_back({"test", 16});
  sh.shaderType = tge::shader::ShaderType::VERTEX;

  getAPILayer()->getShaderAPI()->createShaderPipe(&sh, 1);

  syncMutex.unlock();
  waitForTime();
  exitWaitCheck();
}

TEST(EngineMain, AvocadoTestOne) {
  tge::main::modules.push_back(new TestModule());
  Avocado2Test *av = new Avocado2Test();
  tge::main::modules.push_back(av);

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

  syncMutex.unlock();
  waitForTime();
  exitWaitCheck();
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
  ASSERT_NO_FATAL_FAILURE(requestExit());
  syncMutex.lock();
  ASSERT_TRUE(hasDestroy);
  ASSERT_EQ(err, Error::NONE);
}

TEST(EngineMain, Exit) { exitWaitCheck(); }

TEST(EngineMain, Restart) { ASSERT_EQ(modules.size(), 0); }

TEST(EngineMain, SamplerAndTextures) {
  tge::main::modules.push_back(new TestModule());

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

TEST(EngineMain, SimpleModel) {
  tge::main::modules.push_back(new TestModule());

  ASSERT_EQ(init(), Error::NONE);

  const auto data = tge::util::wholeFile("assets/Triangle.gltf");
  ASSERT_FALSE(data.empty());
  ASSERT_NE(getGameGraphicsModule()->loadModel(data, false, "assets"),
            UINT64_MAX);

  syncMutex.unlock();
  waitForTime();
  exitWaitCheck();
}

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

  for (size_t i = 0; i <= (size_t)MAX_TYPE; i++) {
    ASSERT_NE(apiLayer->loadShader((MaterialType)i), nullptr);
  }

  delete apiLayer;

  ASSERT_EQ(init(), Error::NONE);
  apiLayer = getAPILayer();
  ASSERT_NE(apiLayer, nullptr);

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

TEST(EngineApi, GameAPIChecks) {}

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

      tge::main::modules.push_back(new TestModule());

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
