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
	
	drawRectangle({ { 0,0,0 }, { 1,0,0,1 } }, 0.25,0.25, buffer);

	drawCircle({ { 0,0,0 },{ 1,0,0,0.5 } }, 0.32, 50, buffer);

}
