#pragma once

#include <iostream>
#include "../Error.hpp"
#include "../util/VectorUtil.hpp"
#include "Material.hpp"

namespace tge::gmc {

	/*
     * This is struct holds all the the properties that
	 * do not directly influence the mash it self
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

    /*
     * Holds all the information necessary
     * for an actor to be drawn. Used with createActor.
     *
     * <ul>
     * <li><strong class='atr'>pProperties</strong> describes the properties of this actor</li>
     * <li><strong class='atr'>indexCount</strong> is the count of elements in pIndices</li>
     * <li><strong class='atr'>pIndices</strong> is an array of indices</li>
     * <li><strong class='atr'>vertexCount</strong> is the count of elements in pVertices</li>
     * <li><strong class='atr'>pVertices</strong> is an array of vertices</li>
     * <li><strong class='atr'>material</strong> is the id of the according material</li></ul>
     *
     * <h4>Valid usage</h4>
     * <ul>
     * <li><strong class='atr'>pProperties</strong> must be valid (see ActorProperties)</li>
     * <li><strong class='atr'>indexCount</strong> needs to be greater than zero</li>
     * <li><strong class='atr'>pIndices</strong> must be a valid pointer</li>
     * <li><strong class='atr'>pIndices</strong> array must be at least the size of indexCount</li>
     * <li><strong class='atr'>vertexCount</strong> needs to be greater than zero</li>
     * <li><strong class='atr'>pVertices</strong> must be a valid pointer</li>
     * <li><strong class='atr'>pVertices</strong> array must be at least the size of vertexCount</li>
     */
	struct ActorInputInfo {
		ActorProperties pProperties;
		uint32_t        indexCount;
		uint32_t*       pIndices;
		uint16_t        vertexCount;
		uint8_t*        pVertices;
	};

	/*
	 * Holds the properties for each actor
	 */
	extern std::vector<ActorProperties> properties;
	/*
	 * Holds the count of indices of each actor
	 */
	extern std::vector<uint32_t> countData;
	/*
         * Holds the index offset of each actor
	 * (Offset form the index buffers stating possition)
         */
	extern std::vector<uint32_t> offsetData;
	/*
	 * Holds the offset of the vertices
	 * (This is the offset for the indices)
	 */
	extern std::vector<uint32_t>  vertexOffsetData;

	/*
	 * Loads the model from the file system
	 * 
	 * <strong>Unfinished</strong>
	 */
	void loadModel(File pFile, ResourceDescriptor* pResourceDescriptors, uint32_t pSize);

	/*
	 * Creates the actors by loading everything into intermediate
	 * vectors, before storing it into the command buffer as soon as possible
	 */
	void createActor(ActorInputInfo* pInputInfo, uint32_t pSize);

	/*
	 * Internal method which loads every actor
	 * into the according command buffer
	 */
	void loadToCommandBuffer(VkCommandBuffer pBuffer, uint8_t pLayerId);

}
