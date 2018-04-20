#include "TGEditor.hpp"


int main() {
	Editor editor = Editor();
	std::cout << "Starting Editor" << std::endl;
	Texture tex = {
		"ODST_Helmet.png"
	};
	createTexture(&tex);
	initTGEngine((App*) &editor);
	return 0;
}

void Editor::drawloop(VertexBuffer* buffer)
{
	drawRectangleWithTexture({ {0, 0, 0}, {1,1,1,1} }, 0.5, 0.5, buffer);
	//drawRectangle({ { 0, 0, 0 },{ 1, 1, 1, 1 } }, 1, 1, buffer);
}
