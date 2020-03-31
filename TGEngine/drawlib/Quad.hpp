#pragma once

#include <stdint.h>
#include <glm/glm.hpp>

namespace tge::drw {

	inline void genQuad(float x0, float y0, float x1, float y1, float u0, float v0, float u1, float v1, void* vertexBuffer, void* indexBuffer, uint32_t offset = 0) {
		float* vertexbytes = (float*)vertexBuffer;
		uint32_t* indexbytes = (uint32_t*)indexBuffer;

		// Vertexpoint 1
		vertexbytes[0] = x0;
		vertexbytes[1] = y0;
		vertexbytes[2] = u0;
		vertexbytes[3] = v0;

		// Vertexpoint 2
		vertexbytes[4] = x1;
		vertexbytes[5] = y1;
		vertexbytes[6] = u1;
		vertexbytes[7] = v1;

		// Vertexpoint 3
		vertexbytes[8] = x0;
		vertexbytes[9] = y1;
		vertexbytes[10] = u0;
		vertexbytes[11] = v1;

		// Vertexpoint 4
		vertexbytes[12] = x1;
		vertexbytes[13] = y0;
		vertexbytes[14] = u1;
		vertexbytes[15] = v0;

		// indices
		indexbytes[0] = 0 + offset;
		indexbytes[1] = 1 + offset;
		indexbytes[2] = 2 + offset;
		indexbytes[3] = 1 + offset;
		indexbytes[4] = 0 + offset;
		indexbytes[5] = 3 + offset;
	}

	inline const glm::mat4 genMatrix(const float x, const float y, const float z, const float scalex, const float scaley) {
		return {
			scalex, 0, 0, x,
			0, scaley, 0, y,
			0, 0, z, 0,
			0, 0, 0, 1.0f
		};
	}
}