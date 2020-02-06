#pragma once

#include "../Stdbase.hpp"

extern uint32_t imageIndex;
extern VkFence drawFence;

void startdraw();

void present();

void submit();

void createMutex();

void destroyMutex();