#pragma once

#include "../util/VectorUtil.hpp"
#include "../pipeline/buffer/IndexBuffer.hpp"
#include "../pipeline/buffer/VertexBuffer.hpp"
#include "../drawlib/DrawLib.hpp"
#include "../pipeline/buffer/Texturebuffer.hpp"
#include "../io/Font.hpp"
#include "../game_content/Camera.hpp"

namespace tg_ui {

	/*
	 * Defines the anchor that is used for the component
	 */
	enum Anchor {
		TOP_LEFT, TOP_CENTER, TOP_RIGHT,
		CENTER_LEFT, CENTER, CENTER_RIGHT,
		BOTTOM_LEFT, BOTTOM_CENTER, BOTTOM_RIGHT
	};

	class UIComponent; // See defenition below

	/*
	 * 	Entity class that holds draw and update components
	 */
	SINCE(0, 0, 4)
	class UIEntity {

	public:
		UIEntity(Anchor anchor) : anchor(anchor) {}
		UIEntity(Anchor anchor, glm::vec2 extent) : anchor(anchor), extent(extent) {}
		UIEntity(glm::vec2 position) : UIEntity(position, glm::vec2(0)) {}
		UIEntity(glm::vec2 position, glm::vec2 extent) : local_position(position), extent(extent) {}

		void addComponent(UIComponent* component);
		void addChildren(UIEntity* children);
		void removeComponent(UIComponent* component);
		void removeChildren(UIEntity* children);

		void draw(IndexBuffer* index, VertexBuffer* vertex);
		void update();
		void init();

		bool isEnabled();
		void setEnabled(bool enabled);

		glm::vec2 getPosition();
		glm::vec2 getRenderPosition();
		glm::vec2 getRenderExtent();

	protected:
		UIEntity* parent = nullptr;

		glm::vec2 local_position = glm::vec2(0);
		glm::vec2 extent = glm::vec2(0);
		Anchor anchor = TOP_LEFT;

		std::vector<UIComponent*> components;
		std::vector<UIEntity*> children;

		void onAddTo(UIEntity* parent);
		void onRemoveFrom(UIEntity* parent);

	private:
		bool enabled = true;
	};

	/*
     * 	Component class that holds the actual draw and update calls
     */
	SINCE(0, 0, 4)
	class UIComponent {

	public:
		UIComponent(Anchor anchor) : anchor(anchor) {}

		virtual void draw(IndexBuffer* index, VertexBuffer* vertex); // default implementation, needs override
		virtual void update(); // default implementation, needs override
		virtual void init(); // default implementation, needs override

		void onAddTo(UIEntity* parent);
		void onRemoveFrom(UIEntity* parent);

		bool isEnabled();
		void setEnabled(bool enabled);

	protected:
		UIEntity* parent = nullptr;
		Anchor anchor = CENTER;
		
	private:
		bool enabled = true;
	};

	extern UIEntity ui_scene_entity;
}