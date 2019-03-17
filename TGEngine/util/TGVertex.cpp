#include "TGVertex.hpp"

bool TGVertex::operator==(const TGVertex & lhs)
{
	return this->uv == lhs.uv && this->position == lhs.position && this->normal == lhs.normal;
}