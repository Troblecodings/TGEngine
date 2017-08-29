#pragma once

#include "stdbase.h"
#include "Vertex.h"
#include "Device.h"

namespace Pipeline {

	struct VertexBuffer
	{
		VkBuffer* buffer;
		VkDeviceMemory* memory;
		VkMemoryRequirements* requirements;
		Device* device;
		vector<Vertex>* vertecies;
	};

	/*
     * Creates a VertexBuffer	
	 */
	SINCE(0, 0, 6)
	void createVertexBuffer(VertexBuffer* buffer);

	/*
	 * Filles the given VertexBuffer
	 */
	SINCE(0, 0, 6)
	void fillBuffer(VertexBuffer* buffer);
	
	/*
	 * Destroys the given VertexBuffer
	 */
	SINCE(0, 0, 6)
	void destroyVertexBuffer(VertexBuffer* buffer);

}
