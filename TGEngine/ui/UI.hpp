#pragma once

#include "../util/VectorUtil.hpp"
#include "../pipeline/buffer/IndexBuffer.hpp"
#include "../pipeline/buffer/VertexBuffer.hpp"
#include "../drawlib/DrawLib.hpp"
#include "../pipeline/buffer/Texturebuffer.hpp"
#include "../io/LoadFont.hpp"

namespace tg_ui {

	class UIComponent; // See defenition below

	/*
	 * 	Entity class that holds draw and update components
	 */
	SINCE(0, 0, 4)
	class UIEntity {

	public:
		UIEntity(glm::vec2 position, glm::vec2 extent);
		glm::vec2 local_position;
		glm::vec2 extent;

		void addComponent(UIComponent* component);
		void addChildren(UIEntity* children);
		void removeComponent(UIComponent* component);
		void removeChildren(UIEntity* children);

		void draw(IndexBuffer* index, VertexBuffer* vertex);
		void update(int mouse_x, int mouse_y);

		glm::vec2 getPosition();
		glm::vec2 UIEntity::getRenderPosition();

	protected:
		UIEntity* parent = nullptr;

		std::vector<UIComponent*> components;
		std::vector<UIEntity*> children;

		void onAddTo(UIEntity* parent);
		void onRemoveFrom(UIEntity* parent);
	};

	/*
     * 	Component class that holds the actual draw and update calls
     */
	SINCE(0, 0, 4)
	class UIComponent {

	public:
		virtual void draw(IndexBuffer* index, VertexBuffer* vertex); // default implementation, needs override
		virtual void update(int mouse_x, int mouse_y); // default implementation, needs override

		void onAddTo(UIEntity* parent);
		void onRemoveFrom(UIEntity* parent);

	protected:
		UIEntity* parent;
	};

	extern UIEntity ui_scene_entity;
}