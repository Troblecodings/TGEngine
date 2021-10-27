#include <TGEngine.hpp>
#include <Util.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <graphics/GameGraphicsModule.hpp>
#include <imgui.h>

using namespace tge::main;
using namespace tge::graphics;

class TableTest : public tge::main::Module {
public:
  GameGraphicsModule *ggm;
  float rotation = 0;
  size_t nodeID;
  float scale = 0;
  glm::vec3 pos = glm::vec3(4, -4, 4);
  Light light = Light({10, 5, 0}, {1, 1, 1}, 0.5f);
  bool rotate = false;

  void tick(double time) {
    if (rotate) {
      rotation += (float)time;
      const auto vec =
          glm::vec4(pos, 1) * glm::rotate(rotation, glm::vec3(0, 1, 0));
      ggm->updateScale(nodeID, glm::vec3(scale, scale, scale));
      ggm->updateViewMatrix(glm::lookAt(glm::vec3(0, 0, 0),
                                        glm::vec3(vec.x, vec.y, vec.z),
                                        glm::vec3(0, 1, 0)));
      // ggm->updateCameraMatrix(glm::perspective(glm::radians(75.0f), 1.0f, 1.0f,
      // 100000.0f));
    }
    ggm->getAPILayer()->pushLights(1, &light);
  }
};

float *__tmp;
float *__r;
bool *__rotate;

int main() {
  TableTest *av = new TableTest();
  tge::main::modules.push_back(av);

  init();
  av->ggm = getGameGraphicsModule();

  __tmp = &av->scale;
  __r = &av->pos.x;
  __rotate = &av->rotate;

  getGUIModule()->guicallback = [] {
    ImGui::Begin("test");
    ImGui::SliderFloat("Scale", __tmp, -10.0f, 10.0f, "%.3f");
    ImGui::SliderFloat3("Position", __r, -200.0f, 200.0f, "%.3f");
    ImGui::Checkbox("Rotate", __rotate);
    ImGui::End();
  };

  const auto vec = tge::util::wholeFile("assets/Test/test.gltf");
  const auto mdlID =
      getGameGraphicsModule()->loadModel(vec, false, "assets/Test/");
  av->nodeID = mdlID;
  av->scale = 0.02f;

  return (uint32_t)start();
}
