#pragma once

#include "../../public/Stdbase.hpp"
#include <functional>

extern uint32_t imageIndex;
extern VkFence drawFence;

extern std::vector<std::function<void()>> executionQueue;

void startdraw();

void present();

void submit();

void createMutex();

void destroyMutex();