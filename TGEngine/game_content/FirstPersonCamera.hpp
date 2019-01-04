#pragma once

#include "../Stdbase.hpp"
#include "Camera.hpp"

void createFirstPersonCamera(Camera* camera);

void __impl_mouseinputhandlerFPC(Camera* camera, glm::vec2 pos, glm::vec2 delta);