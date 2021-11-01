#include <IO/IOModule.hpp>
#include <TGEngine.hpp>
#include <Util.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <graphics/GUIModule.hpp>
#include <graphics/GameGraphicsModule.hpp>
#include <imgui.h>

using namespace tge::main;
using namespace tge::graphics;

class TableTest : public tge::main::Module {
public:
  GameGraphicsModule *ggm;
  float rotation = 0;
  size_t nodeID;
  glm::vec3 direction = glm::vec3(4, -4, 4);
  glm::vec3 pos = glm::vec3(4, -4, 4);
  Light light = Light({10, 5, 0}, {1, 1, 1}, 0.5f);
  bool rotate = false;

  void tick(double time) {
    if (rotate) {
      rotation += (float)time;
      ggm->updateScale(nodeID, glm::vec3(0.2f, 0.2f, 0.2f));
      auto dir =
          glm::rotate(glm::vec3(1, 1, 1), direction.x, glm::vec3(1, 0, 0));
      dir = glm::rotate(dir, direction.y, glm::vec3(0, 0, 1));
      ggm->updateCameraMatrix(
          glm::lookAt(pos, pos - direction, glm::vec3(0, 1, 0)));
    }
    ggm->getAPILayer()->pushLights(1, &light);
  }
};

class TestRenderGUI : public tge::gui::GUIModule {

public:
  float *__tmp;
  float *__r;
  bool *__rotate;

  float *lightPos;

  void renderGUI() {
    ImGui::Begin("test");
    ImGui::SliderFloat3("Dir", __tmp, -2.0f, 2.0f, "%.3f");
    ImGui::SliderFloat3("Position", __r, -4.0f, 4.0f, "%.3f");
    ImGui::SliderFloat3("Light Pos", lightPos, -5, 5, "%.3f");
    ImGui::Checkbox("Rotate", __rotate);
    ImGui::End();
  }
};

class TestIOMode : public tge::io::IOModule {
public:
  TableTest *tt;
  int x;
  int y;

  TestIOMode(TableTest *tt) : tt(tt) {}

  void mouseEvent(const tge::io::MouseEvent event) { 
    tt->direction.x -= (event.x - x) * 0.01;
    tt->direction.y += (event.y - y) * 0.01;
    x = event.x;
    y = event.y;
  }

  void keyboardEvent(const tge::io::KeyboardEvent event) {
    printf("KeyboardEvent %i", event.signal);
  }
};

int main() {
  TableTest *av = new TableTest();
  tge::main::lateModules.push_back(av);
  TestRenderGUI *gui = new TestRenderGUI();
  tge::main::lateModules.push_back(gui);
  TestIOMode *iotest = new TestIOMode(av);
  tge::main::lateModules.push_back(iotest);

  gui->__tmp = &av->direction.x;
  gui->__r = &av->pos.x;
  gui->__rotate = &av->rotate;
  gui->lightPos = &av->light.pos.x;

  const auto res = init();
  if (res != Error::NONE) {
    printf("Error in init!");
    return -1;
  }

  av->ggm = getGameGraphicsModule();

  const auto vec = tge::util::wholeFile("assets/Test/test.gltf");
  const auto mdlID =
      getGameGraphicsModule()->loadModel(vec, false, "assets/Test/");
  av->nodeID = mdlID;

  const auto startRes = start();
  if (startRes != Error::NONE) {
    printf("Error in init!");
    return -1;
  }

  return (uint32_t)startRes;
}
