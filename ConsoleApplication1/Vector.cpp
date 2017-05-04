#include "stdafx.h"
#include "Vector.h"


float x, y, z;



Vector::Vector(float ix,float iy,float iz)
{
	x = ix;
	y = iy;
	z = iz;
}

float* Vector::getAsArray() {
	float arr[] = { x,y,z };
	return arr;
}

Vector::~Vector()
{
}
