#include "UIButton.h"

UIButton::UIButton()
{
	this->draw_call = &drawButton;
}

void drawButton(UIComponent* comp, IndexBuffer* idx, VertexBuffer* vert) {
	drawRectangle({
		{0, 0, 0},
		{1, 0, 0, 1},
	    {0, 0},
		COLOR_ONLY
		}, 0.5, 0.5, vert, idx);
}
