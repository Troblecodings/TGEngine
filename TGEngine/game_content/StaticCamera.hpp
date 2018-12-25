#pragma once

#include "../Stdbase.hpp"
#include "Camera.hpp"

void createStaticCamera(Camera* camera);

void __impl_mouseinputHandleSC(glm::vec2 pos, glm::vec2 delta, Camera* camera);