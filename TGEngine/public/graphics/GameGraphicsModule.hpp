#pragma once

#include "../../public/Error.hpp"
#include "../../public/Module.hpp"
#include "APILayer.hpp"
#include "GameShaderModule.hpp"
#include "Material.hpp"
#include "WindowModule.hpp"
#include "stdint.h"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <string>
#include <vector>

namespace tge::graphics {

struct NodeTransform {
  glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
  glm::quat rotation = glm::quat(0.0f, 0.0f, 0.0f, 0.0f);
};

struct NodeInfo {
  size_t bindingID = UINT64_MAX;
  NodeTransform transforms = {};
  size_t parent = UINT64_MAX;
};

class GameGraphicsModule : public main::Module {

  APILayer *apiLayer;
  WindowModule *windowModule;

  glm::mat4 projectionMatrix;
  glm::mat4 viewMatrix;
  std::vector<glm::mat4> modelMatrices;
  std::vector<NodeTransform> node;
  std::vector<size_t> parents;
  std::vector<size_t> bindingID;
  std::vector<char> status; // jesus fuck not going to use a bool here
  size_t dataID = UINT64_MAX;

public:
  size_t defaultMaterial;
  tge::shader::ShaderPipe defaultPipe;

  GameGraphicsModule(APILayer *apiLayer, WindowModule *winModule);

  _NODISCARD size_t loadModel(const std::vector<char> &data, const bool binary,
                              const std::string &baseDir,
                              void *shaderPipe = nullptr);

  _NODISCARD size_t loadModel(const std::vector<char> &data,
                              const bool binary) {
    return loadModel(data, binary, "");
  }

  _NODISCARD uint32_t loadTextures(const std::vector<std::vector<char>> &data);

  _NODISCARD uint32_t loadTextures(const std::vector<std::string> &names);

  _NODISCARD size_t addNode(const NodeInfo *nodeInfos, const size_t count);

  void updateTransform(const size_t nodeID, const NodeTransform &transform);

  void updateViewMatrix(const glm::mat4 matrix) { this->viewMatrix = matrix; }

  void updateCameraMatrix(const glm::mat4 matrix) {
    this->projectionMatrix = matrix;
  }

  main::Error init() override;

  void tick(double time) override;

  void destroy() override;

  _NODISCARD APILayer *getAPILayer() { return apiLayer; }

  _NODISCARD WindowModule *getWindowModule() { return windowModule; }
};

} // namespace tge::graphics