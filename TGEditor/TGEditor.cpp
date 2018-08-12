#include "TGEditor.hpp"

Texture tex1;
Texture tex2;
fbxsdk::FbxMesh* mesh;
Font arial;

int main() {
	Editor editor = Editor();
	std::cout << "Starting Editor" << std::endl;

	arial = {
		"arial.ttf"
	};
	loadfont(&arial);


	mesh = load("lul.fbx");

	tex1 = {
		"ODST_Helmet.png"
	};
	tex2 = {
		"LogoTM.png"
	};
	createTexture(&tex1);
	createTexture(&tex2);
	initTGEngine((App*)&editor);
	return 0;
}

void drawLineH(TGVertex start, float length, VertexBuffer* buffer) {
	drawRectangle(start, length, 0.002, buffer);
	drawRectangle(start, -length, 0.002, buffer);
}

void drawLineV(TGVertex start, float length, VertexBuffer* buffer) {
	drawRectangle(start, 0.002, length, buffer);
	drawRectangle(start, 0.002, -length, buffer);
}

void drawGrid(VertexBuffer* buffer) {
	for (uint32_t i = 0; i < 41; i++) {
		drawLineH({ { 0, -2 + 0.1 * i, 0 }, { 0, 0, 0, 1}, {0, 0}, COLOR_ONLY }, 2, buffer);
		drawLineV({ { -2 + 0.1 * i, 0, 0 },{ 0, 0, 0, 1 },{ 0, 0 }, COLOR_ONLY }, 2, buffer);
	}
	drawRectangleZ({ { 0, 0, 0.15 },{ 1, 0, 0, 1 }, { 0, 0 }, COLOR_ONLY }, 0.002, 0.15, buffer);
	drawRectangleZ({ { 0, 0, 0.15 },{ 1, 0, 0, 1 }, { 0, 0 }, COLOR_ONLY }, -0.002, 0.15, buffer);
	drawRectangle({ { 0, 0.15, 0},{ 0, 1, 0, 1 }, { 0, 0 }, COLOR_ONLY }, 0.002, 0.15, buffer);
	drawRectangle({ { 0, 0.15, 0},{ 0, 1, 0, 1 }, { 0, 0 }, COLOR_ONLY }, -0.002, 0.15, buffer);
	drawRectangle({ { 0.15, 0, 0 },{ 0, 0, 1, 1 },{ 0, 0 }, COLOR_ONLY }, 0.15, 0.002, buffer);
	drawRectangle({ { 0.15, 0, 0 },{ 0, 0, 1, 1 },{ 0, 0 }, COLOR_ONLY }, 0.15, -0.002, buffer);
}

void Editor::drawloop(VertexBuffer* buffer)
{
	arial.drawString({ {-1, 0, 0},  { 0, 0, 0, 1} }, "Hallo Welt!", buffer);
}
