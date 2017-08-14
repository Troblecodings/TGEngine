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

	void createVertexBuffer(VertexBuffer* buffer);

	void fillBuffer(VertexBuffer* buffer);

	void destroyVertexBuffer(VertexBuffer* buffer);

}
