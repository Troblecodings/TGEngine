#include "TGVertex.hpp"

bool TGVertex::operator==(const TGVertex & lhs)
{
	return this->color == lhs.color && this->uv == lhs.uv && this->position == lhs.position && this->color_only == lhs.color_only && this->normal == lhs.normal;
}