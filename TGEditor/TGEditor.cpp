#include "TGEditor.hpp"

Texture tex1;
Texture tex2;
Mesh mesh;
Font arial;
Camera camera;

int main() {
	Editor editor = Editor();
	std::cout << "Starting Editor" << std::endl;
	initTGEngine(&editor.main_window, &drawloop, &init);
	std::cout << "Clean exit! Bye :wave:!" << std::endl;
	return 0;
}

void drawLineH(TGVertex start, float length, VertexBuffer* buffer) {
	drawRectangle(start, length, 0.002f, buffer);
	drawRectangle(start, -length, 0.002f, buffer);
}

void drawLineV(TGVertex start, float length, VertexBuffer* buffer) {
	drawRectangle(start, 0.002f, length, buffer);
	drawRectangle(start, 0.002f, -length, buffer);
}

void drawGrid(VertexBuffer* buffer) {
	for (uint32_t i = 0; i < 41; i++) {
		drawLineH({ { 0, -2 + 0.1 * i, 0 }, { 0, 0, 0, 1}, {0, 0}, COLOR_ONLY }, 2, buffer);
		drawLineV({ { -2 + 0.1 * i, 0, 0 },{ 0, 0, 0, 1 },{ 0, 0 }, COLOR_ONLY }, 2, buffer);
	}
	drawRectangleZ({ { 0, 0, 0.15 },{ 1, 0, 0, 1 }, { 0, 0 }, COLOR_ONLY }, 0.002f, 0.15f, buffer);
	drawRectangleZ({ { 0, 0, 0.15 },{ 1, 0, 0, 1 }, { 0, 0 }, COLOR_ONLY }, -0.002f, 0.15f, buffer);
	drawRectangle({ { 0, 0.15, 0},{ 0, 1, 0, 1 }, { 0, 0 }, COLOR_ONLY }, 0.002f, 0.15f, buffer);
	drawRectangle({ { 0, 0.15, 0},{ 0, 1, 0, 1 }, { 0, 0 }, COLOR_ONLY }, -0.002f, 0.15f, buffer);
	drawRectangle({ { 0.15, 0, 0 },{ 0, 0, 1, 1 },{ 0, 0 }, COLOR_ONLY }, 0.15f, 0.002f, buffer);
	drawRectangle({ { 0.15, 0, 0 },{ 0, 0, 1, 1 },{ 0, 0 }, COLOR_ONLY }, 0.15f, -0.002f, buffer);
}

void init() {
	arial = {
	    "resource\\arial.ttf"
	};
	loadfont(&arial);

	loadFromFBX(&mesh,"resource\\lul.fbx");

	tex1 = {
		"resource\\ODST_Helmet.png"
	};
	tex2 = {
		"resource\\LogoTM.png"
	};
	createTexture(&tex1);
	createTexture(&tex2);

	createFirstPersonCamera(&camera);
}

void drawloop(IndexBuffer* ibuffer, VertexBuffer* vbuffer)
{
	ibuffer->addIndex(0);
	ibuffer->addIndex(1);
	ibuffer->addIndex(2);

	vbuffer->add({
		{-1, -1, 0},
		{1, 0, 0, 1},
		{0, 0},
		COLOR_ONLY
	});
	vbuffer->add({
	    {1, 0, 0},
	    {1, 0, 0, 1},
	    {0, 0},
	    COLOR_ONLY
	});
	vbuffer->add({
		{0, 1, 0},
		{1, 0, 0, 1},
		{0, 0},
		COLOR_ONLY
	});

	mesh.consume(vbuffer, ibuffer);
	//drawGrid(buffer);
	//drawRectangleWithTexture({ {0.2, 0.2, 0.2},  { 1, 1, 1, 1}, {0, 0}, tex2.index }, 0.3, 0.3, buffer);
	//drawRectangleWithTexture({ {0, 0, 0},  { 1, 1, 1, 1}, {0, 0}, tex2.index }, 0.3, 0.3, buffer);
	//arial.drawString({ {0, 0, -0.3},  { 1, 0, 0, 1} }, "Hallo Welt!", buffer);
}
