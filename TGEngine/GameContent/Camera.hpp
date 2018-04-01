#pragma once
#include "..\stdbase.hpp"
#include "..\Pipeline\Buffer\VertexBuffer.hpp"

struct Rotation {
	float x_rotation,
		y_rotation,
		z_rotation,
		origin_x,
		origin_y,
		origin_z;

	SINCE(0, 0, 2)
	void setRotations(float x, float y, float z);
};

struct Camera {

	INPUT Rotation rotation;
	INPUT VertexBuffer buffer;

	SINCE(0, 0, 2)
	void updateCamera();
};

SINCE(0, 0, 2)
void createCamera(Camera* camera);