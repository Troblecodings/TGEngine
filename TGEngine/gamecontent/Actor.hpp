#pragma once

#include <iostream>
#include "../Error.hpp"
#include "../util/VectorUtil.hpp"
#include "Material.hpp"

namespace tge::gmc {

	/*
     * This is struct holds all the the properties that
	 * do not directly influence the mesh it self
     *
     * <ul>
     * <li><strong class='atr'>localTransform</strong> is the local tranform of the actor that will be computed at runtime</li>
     * <li><strong class='atr'>material</strong> is the index of the material used to draw this mesh</li>
     * <li><strong class='atr'>layer</strong> is the layer the actor is drawn on (0 for world or 1 for UI)</li></ul>
     *
     * <h4>Valid usage</h4>
     * <ul>
     * <li><strong class='atr'>localTransform</strong> must be a non-zero matrix</li>
     * <li><strong class='atr'>material</strong> must be a valid material id in the material list</li>
     * <li><strong class='atr'>material</strong> must be accessable in the given layer</li>
     * <li><strong class='atr'>layer</strong> must be a valid layer id</li></ul>
     */
	struct ActorProperties {
		glm::mat4 localTransform;
		uint8_t   material;
		uint8_t   layer;
	};

	struct ActorDescriptor {
		uint32_t indexDrawCount;
		uint32_t indexOffset;
		uint32_t vertexOffset;
	};

	/*
	 * Holds the properties for each actor
	 */
	extern std::vector<ActorProperties> actorProperties;
	/*
     * Holds the description of an actor
     */
	extern std::vector<ActorDescriptor> actorDescriptor;

	/*
	 * Internal method which loads every actor
	 * into the according command buffer
	 */
	void loadToCommandBuffer(VkCommandBuffer pBuffer, uint8_t pLayerId);

}
