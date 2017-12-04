#define DEBUG
#include "TGEditor.hpp"


int main() {
	Editor editor = {};
	std::cout << "Starting Editor" << std::endl;
	initTGEngine(&editor);
	return 0;
}

void Editor::drawloop(std::vector<Vertex>* verticies)
{
	if (verticies->size() < 10) {
		drawCircle({ {0,0,0}, {1,0,0} }, 0.5, 500, verticies);
	}
}
