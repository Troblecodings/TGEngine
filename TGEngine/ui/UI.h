#pragma once

#include "../util/VectorUtil.hpp"
#include "../pipeline/buffer/IndexBuffer.hpp"
#include "../pipeline/buffer/VertexBuffer.hpp"
#include "../drawlib/DrawLib.hpp"
#include "../pipeline/buffer/Texturebuffer.hpp"
#include "../io/LoadFont.hpp"

struct UIComponent {

	std::vector<void*> ui_children;

	void (*draw_call)(void*, IndexBuffer*, VertexBuffer*) = nullptr;
	void (*update_call)(void*, int, int) = nullptr;

	void draw(IndexBuffer* index, VertexBuffer* vertex);
	void update(int mouse_x, int mouse_y);
};

extern UIComponent scene_component;