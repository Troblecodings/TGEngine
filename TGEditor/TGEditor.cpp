#include "TGEditor.hpp"

Texture tex1;
Texture tex2;
Mesh mesh;
Mesh mesh2;
Font arial;
Camera camera;
UIButton* btn;
UITexture* tex;

int main() {
	Editor editor = Editor();
	std::cout << "Starting Editor" << std::endl;
	initTGEngine(&editor.main_window, &drawloop, &init);
	std::cout << "Clean exit! Bye :wave:!" << std::endl;
	return 0;
}

void drawLineH(TGVertex start, float length, VertexBuffer* buffer, IndexBuffer* vbuffer) {
	drawRectangle(start, length, 0.002f, buffer, vbuffer);
	drawRectangle(start, -length, 0.002f, buffer, vbuffer);
}

void drawLineV(TGVertex start, float length, VertexBuffer* buffer, IndexBuffer* vbuffer) {
	drawRectangle(start, 0.002f, length, buffer, vbuffer);
	drawRectangle(start, 0.002f, -length, buffer, vbuffer);
}

void drawGrid(VertexBuffer* buffer, IndexBuffer* vbuffer) {
	for (uint32_t i = 0; i < 41; i++) {
		drawLineH({ { 0, -2 + 0.1 * i, 0 }, { 0, 0, 0, 1}, {0, 0}, COLOR_ONLY }, 2, buffer, vbuffer);
		drawLineV({ { -2 + 0.1 * i, 0, 0 },{ 0, 0, 0, 1 },{ 0, 0 }, COLOR_ONLY }, 2, buffer, vbuffer);
	}
	drawRectangleZ({ { 0, 0, 0.15 },{ 1, 0, 0, 1 }, { 0, 0 }, COLOR_ONLY }, 0.002f, 0.15f, buffer, vbuffer);
	drawRectangleZ({ { 0, 0, 0.15 },{ 1, 0, 0, 1 }, { 0, 0 }, COLOR_ONLY }, -0.002f, 0.15f, buffer, vbuffer);
	drawRectangle({ { 0, 0.15, 0},{ 0, 1, 0, 1 }, { 0, 0 }, COLOR_ONLY }, 0.002f, 0.15f, buffer, vbuffer);
	drawRectangle({ { 0, 0.15, 0},{ 0, 1, 0, 1 }, { 0, 0 }, COLOR_ONLY }, -0.002f, 0.15f, buffer, vbuffer);
	drawRectangle({ { 0.15, 0, 0 },{ 0, 0, 1, 1 },{ 0, 0 }, COLOR_ONLY }, 0.15f, 0.002f, buffer, vbuffer);
	drawRectangle({ { 0.15, 0, 0 },{ 0, 0, 1, 1 },{ 0, 0 }, COLOR_ONLY }, 0.15f, -0.002f, buffer, vbuffer);
}

void init() {
	arial = {
	    "resource\\arial.ttf",
		60.0f
	};
	loadfont(&arial);

	loadFromFBX(&mesh, "resource\\loop.fbx");
	loadFromFBX(&mesh2, "resource\\lul.fbx");

	tex1 = {
		"resource\\ODST_Helmet.png"
	};
	tex2 = {
		"resource\\LogoTM.png"
	};
	createTexture(&tex1);
	createTexture(&tex2);

	createActor(&mesh)->preRotate(PI / 2, 1.0f, 0, 0)->preScale(0.5f, 0.5f, 0.5f)->prePos(0, 1.0f, 0)->applyPretransform();
	createActor(&mesh2)->prePos(0, 1, 0)->applyPretransform();

	createStaticCamera(&camera);

	btn = new UIButton(&arial, { 0.1, 0.1, 0.2, 0.2 }, { 0, 0, 0, 0.5 }, "Play");
	tex = new UITexture({ -0.8, -0.8, 0.1, 0.1 }, &tex1);
	TG_VECTOR_APPEND_NORMAL(scene_component.ui_children, btn)
	TG_VECTOR_APPEND_NORMAL(scene_component.ui_children, tex)
}

void drawloop(IndexBuffer* ibuffer, VertexBuffer* vbuffer)
{
	drawGrid(vbuffer, ibuffer);
	arial.drawString({
		{0, 0, 0},
		{1, 1, 0, 1},
		}, "Play", vbuffer, ibuffer);
}
