#pragma once

#include "../util/VectorUtil.hpp"
#include "../pipeline/buffer/IndexBuffer.hpp"
#include "../pipeline/buffer/VertexBuffer.hpp"
#include "../drawlib/DrawLib.hpp"
#include "../pipeline/buffer/Texturebuffer.hpp"
#include "../io/LoadFont.hpp"

union TGRect{
	float x, y, width, height;
};

struct UIEntity {

	struct UIComponent {

		UIEntity* parent;

		virtual void draw(IndexBuffer* index, VertexBuffer* vertex) = 0;
		virtual void update(int mouse_x, int mouse_y) = 0;

		void onAddTo(UIEntity* parent);
		void onRemoveFrom(UIEntity* parent);
	};

	UIEntity(TGRect rect);

	UIEntity* parent = nullptr;
	TGRect local_bounds;

	std::vector<UIComponent*> components;
	std::vector<UIEntity*> children;

	void addComponent(UIComponent* component);
	void addChildren(UIEntity* children);
	void removeComponent(UIComponent* component);
	void removeChildren(UIEntity* children);

	void draw(IndexBuffer* index, VertexBuffer* vertex);
	void update(int mouse_x, int mouse_y);

	void onAddTo(UIEntity* parent);
	void onRemoveFrom(UIEntity* parent);

	TGRect getBounds();
};

extern UIEntity ui_scene_entity;