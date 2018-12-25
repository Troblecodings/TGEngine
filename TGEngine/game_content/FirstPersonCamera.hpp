#pragma once

#include "../Stdbase.hpp"
#include "Camera.hpp"

void createFirstPersonCamera(Camera* camera);

void __impl_mouseinputhandlerFPC(glm::vec2 pos, glm::vec2 delta, Camera* camera);