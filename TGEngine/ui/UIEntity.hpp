#pragma once

#include "../util/VectorUtil.hpp"
#include "../pipeline/buffer/IndexBuffer.hpp"
#include "../pipeline/buffer/VertexBuffer.hpp"
#include "../drawlib/DrawLib.hpp"
#include "../pipeline/buffer/Texturebuffer.hpp"
#include "../io/Font.hpp"
#include "../game_content/Camera.hpp"

namespace tge {
	namespace ui {

		/*
		 * Defines the anchor that is used for the component
		 */
		enum Anchor {
			TOP_LEFT, TOP_CENTER, TOP_RIGHT,
			CENTER_LEFT, CENTER, CENTER_RIGHT,
			BOTTOM_LEFT, BOTTOM_CENTER, BOTTOM_RIGHT
		};

		class UIDrawable; // See defenition in UIDrawable.hpp
		class UIUpdatable; // See defenition in UIUpdatable.hpp

		/*
		 * 	Entity class that holds draw and update components
		 */
		SINCE(0, 0, 4)
		class UIEntity {

		private:
			// The parent entity (can be null for none)
			UIEntity* parent = nullptr;

			// Sub Entitys/Drawables/Updatables
			std::vector<UIDrawable*> drawComponents;
			std::vector<UIUpdatable*> updateComponents;
			std::vector<UIEntity*> children;

			// Local position and extent
			glm::vec2 localPosition = glm::vec2(0);
			glm::vec2 extent = glm::vec2(0);

			// Cached postion and extent-> Calculated on position/extent change
			glm::vec2 cachedPosition = glm::vec2(0);
			glm::vec2 cachedExtent = glm::vec2(0);

			Anchor anchor = TOP_LEFT;
			bool enabled = true; // if not enabled this wont draw nor update
			bool visible = true; // if not visible this wont draw but will still update

		public:
			UIEntity(Anchor anchor) : anchor(anchor) {}
			UIEntity(Anchor anchor, glm::vec2 extent) : anchor(anchor), extent(extent) {}
			UIEntity(glm::vec2 position) : UIEntity(position, glm::vec2(0)) {}
			UIEntity(glm::vec2 position, glm::vec2 extent) : localPosition(position), extent(extent) {}

			// add/remove sub components/entitys
			void addDrawable(UIDrawable* component);
			void addUpdatable(UIUpdatable* component);
			void addChildren(UIEntity* children);
			
			void removeDrawable(UIDrawable* component);
			void removeUpdatable(UIUpdatable* component);
			void removeChildren(UIEntity* children);
			//

			void init();
			void update();
			void draw(IndexBuffer* index, VertexBuffer* vertex);

			// See enabled
			bool isEnabled();
			void setEnabled(bool enabled);
			// See visible
			bool isVisible();
			void setVisible(bool visible);

			// Getter for cached values
			glm::vec2 getPosition();
			glm::vec2 getExtent();

			// Getter for unprocessed values
			glm::vec2 getLocalPosition();
			glm::vec2 getLocalExtent();

			// Setter for locals
			// Updates cached values
			void setPosition(glm::vec2 position);
			void setExtent(glm::vec2 extent);

		protected:
			void onAddTo(UIEntity* parent);
			void onRemoveFrom(UIEntity* parent);

		private:
			INTERNAL
			glm::vec2 __getPosition();
			void __update();
		};

		extern UIEntity ui_scene_entity;
	}
}