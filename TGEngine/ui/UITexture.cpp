#include "UITexture.hpp"

UITexture::UITexture(glm::vec4 rect, char* string)
{
	this->tex = {
		string
	};
	createTexture(&this->tex);
}

UITexture::UITexture(glm::vec4 rect, Texture* tex)
{
	this->tex = *tex;
}