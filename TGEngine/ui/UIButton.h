#pragma once

#include "UI.h"

struct UIButton : UIComponent
{
	UIButton();
};

void drawButton(UIComponent* comp, IndexBuffer* idx, VertexBuffer* vert);
