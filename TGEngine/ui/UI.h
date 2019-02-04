#pragma once

#include "../util/VectorUtil.hpp"
#include "../pipeline/buffer/IndexBuffer.hpp"
#include "../pipeline/buffer/VertexBuffer.hpp"
#include "../drawlib/DrawLib.hpp"
#include "../pipeline/buffer/Texturebuffer.hpp"

struct UIComponent {

	std::vector<UIComponent> ui_children;

	void (*draw_call)(UIComponent*, IndexBuffer*, VertexBuffer*) = nullptr;
	void (*update_call)(UIComponent*, int, int) = nullptr;

	void draw(IndexBuffer* index, VertexBuffer* vertex);
	void update(int mouse_x, int mouse_y);
};

extern UIComponent scene_component;