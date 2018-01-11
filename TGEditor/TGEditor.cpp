#include "TGEditor.hpp"


int main() {
	Editor editor = {};
	std::cout << "Starting Editor" << std::endl;
	initTGEngine(&editor);
	//loadfont("arial.ttf");
	return 0;
}

void Editor::drawloop(VertexBuffer* buffer)
{

	drawCircle({
		{0, 0, 0},
		{1, 0, 0}
	}, 0.5, 500, buffer);

}
