#pragma once

#include <vector>
#include <list>
#include <glm/glm.hpp>
#include "../pipeline/buffer/VertexBuffer.hpp"
#include "../pipeline/buffer/IndexBuffer.hpp"
#include "../util/VectorUtil.hpp"
#include "../gamecontent/Material.hpp"
#include "../pipeline/Shader.hpp"
#include "../pipeline/Pipe.hpp"

namespace tge {
	namespace gmc {

		struct Mesh {
			std::vector<TGVertex> vertices;
			std::vector<uint32_t> indices;

			/*
			 * this is used in the internal draw -> adds the mesh to the buffer
			 */
			SINCE(0, 0, 4)
				void consume(VertexBuffer* vrt, IndexBuffer* ind);

			/*
			 * This adds a vertex to this mesh [same as add(TGVertex vrt)]
			 */
			SINCE(0, 0, 4)
				void operator<<(TGVertex vrt);

			/*
			 * This adds a vertex to this mesh or the index of the vertex if it should already exist
			 */
			SINCE(0, 0, 4)
				void add(TGVertex vrt);
		};
	}
}