#pragma once
#include "..\stdbase.hpp"
#include "..\Pipeline\Buffer\UniformBuffer.hpp"

struct Camera {

	INPUT float x, y;
	OUTPUT 

	SINCE(0, 0, 2)
	void updateCamera();
};