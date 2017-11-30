#include "TGEditor.hpp"


int main() {
	Editor editor = {};
	initTGEngine(&editor);
	return 0;
}

void Editor::drawloop(std::vector<Vertex>* verticies)
{
	drawCircle({ {0,0,0}, {1,0,0} }, 0.5, 500, verticies);
}
