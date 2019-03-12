#include "TGVertex.hpp"

bool TGVertex::operator==(const TGVertex & lhs)
{
	return this->color == lhs.color && this->uv == lhs.uv && this->position == lhs.position && this->color_only == lhs.color_only && this->normal == lhs.normal;
}

void TGVertex::operator>>(Renderer* render)
{
	for (size_t b = 0; b < render->vbuffer->size(); b++)
	{
		if ((*render->vbuffer)[b] == *this) {
			render->ibuffer->push_back((uint32_t)b);
			return;
		}
	}
	last_size = (uint32_t)render->vbuffer->size();
	render->vbuffer->push_back(*this);
	render->ibuffer->push_back((uint32_t)last_size);
}
