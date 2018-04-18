#include "TGEditor.hpp"


int main() {
	Editor editor = {};
	std::cout << "Starting Editor" << std::endl;
	Texture tex = {
		"ODST_Helmet.png"
	};
	createTexture(&tex);
	initTGEngine(&editor);
	return 0;
}

void Editor::drawloop(VertexBuffer* buffer)
{
	drawRectangleWithTexture({ {0, 0, 0}, {1,1,1,1} }, 1, 1, buffer);
	//drawRectangle({ { 0, 0, 0 },{ 1, 1, 1, 1 } }, 1, 1, buffer);
}
