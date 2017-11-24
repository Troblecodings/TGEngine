#include "Editor.hpp"


void main() {
	EditorApp app = {};
	initTGEngine(&app);
}

void EditorApp::drawloop(std::vector<Vertex>* verticies)
{
	if (verticies->size() < 10){
		drawCircle({
			{ 0, 0, 0 },
			{ 1.0F, 0,0 }
		}, 0.5, 100, verticies);
		drawCircle({
		    {1,0,0},
		    {1.0F,0,0}
	    }, 0.5, 100, verticies);

		drawCircle({
			{ 0,1,0 },
			{ 1.0F,0,0 }
		}, 0.5, 100, verticies);
    }
}
