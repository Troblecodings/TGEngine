#include "TGEditor.hpp"

Texture tex1;
Texture tex2;
Mesh mesh;
Mesh mesh2;
Font arial;
Camera camera;
tg_ui::UIEntity entity = tg_ui::UIEntity({0, 0}, {0.3, 0.15});

int main() {
	Editor editor = Editor();
	std::cout << "Starting Editor" << std::endl;

	tg_ui::UITextureComponent texture = tg_ui::UITextureComponent(&tex2);
	entity.addComponent(&texture);
	tg_ui::ui_scene_entity.addChildren(&entity);

	initTGEngine(&editor.main_window, &drawloop, &init);
	std::cout << "Clean exit! Bye :wave:!" << std::endl;
	return 0;
}

void init() {
	arial = {
	    "resource\\arial.ttf",
		60.0f
	};
	loadfont(&arial);

	loadFromFBX(&mesh, "resource\\loop.fbx");
	loadFromFBX(&mesh2, "resource\\schachbrett.fbx");

	tex1 = {
		"resource\\ODST_Helmet.png"
	};
	tex2 = {
		"resource\\test_logo.png"
	};
	createTexture(&tex1);
	createTexture(&tex2);

	createActor(&mesh)->preRotate(PI / 2, 1.0f, 0, 0)->preScale(0.5f, 0.5f, 0.5f)->prePos(0, 1.0f, 0)->applyPretransform();
	//createActor(&mesh2)->preScale(0.4, 0.4, 0.4)->applyPretransform();

	createFirstPersonCamera(&camera);
}

void drawloop(IndexBuffer* ibuffer, VertexBuffer* vbuffer)
{

}
