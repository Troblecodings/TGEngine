#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <iostream>
#include "../Error.hpp"
#include "../util/VectorUtil.hpp"
#include "Material.hpp"

namespace tge::gmc {
	
	/*
	 * This is struct holds all the information necessary
	 * for an actor to be drawn
	 * 
	 * use createActor to create the actors from this
	 * struct(s)
	 * <ul>
	 * <li><strong class='atr'>localTransform</strong> is the local tranform that is calculated</li>
	 * <li><strong class='atr'>indexCount</strong> is the count of elements in pIndices</li>
	 * <li><strong class='atr'>pIndices</strong> is an array of indices</li>
	 * <li><strong class='atr'>vertexCount</strong> is the count of elements in pVertices</li>
	 * <li><strong class='atr'>pVertices</strong> is an array of vertices</li>
	 * <li><strong class='atr'>material</strong> is the id of the according material</li></ul>
	 *
	 * <h4>Valid usage</h4>
	 * <ul>
	 * <li><strong class='atr'>indexCount</strong> needs to be greater than zero</li>
	 * <li><strong class='atr'>pIndices</strong> must be a valid pointer</li>
	 * <li><strong class='atr'>pIndices</strong> array must be at least the size of indexCount</li>
	 * <li><strong class='atr'>vertexCount</strong> needs to be greater than zero</li>
	 * <li><strong class='atr'>pVertices</strong> must be a valid pointer</li>
	 * <li><strong class='atr'>pVertices</strong> array must be at least the size of vertexCount</li>
	 * <li><strong class='atr'>material</strong> must be in the range form 0 to 255</li>
	 * <li><strong class='atr'>material</strong> must be a valid index in the createdMaterials arrays</li></ul>
	 */
	struct ActorInputInfo {
		glm::mat4 localTransform;
		uint32_t  indexCount;
		uint32_t* pIndices;
		uint16_t  vertexCount;
		uint8_t*  pVertices;
		// TODO copy material
		uint8_t   material;
	};

	/*
	 * This holds the local tranforms of each actor
	 */
	extern std::vector<glm::mat4> localTranformIds;
	/*
	 * This holds the count of indices of each actor
	 */
	extern std::vector<uint32_t> countData;
	/*
     * This holds the index offset of each actor
	 * (Offset form the index buffers stating possition)
     */
	extern std::vector<uint32_t> offsetData;
	/*
	 * This holds the material ids for each actor
	 */
	extern std::vector<uint8_t>  materialIds;

	/*
	 * Loads the model from the disk
	 * 
	 * <strong>Unfinished</strong>
	 */
	void loadModel(File pFile, ResourceDescriptor* pResourceDescriptors, uint32_t pSize);

	/*
	 * Creates the actors by loading everything into intermedate
	 * vectors before storing it into the command buffer asap
	 */
	void createActor(ActorInputInfo* pInputInfo, uint32_t pSize);

	/*
	 * This is an internal method which loads evry actor
	 * into the according command buffer
	 */
	void loadToCommandBuffer(VkCommandBuffer pBuffer);

}