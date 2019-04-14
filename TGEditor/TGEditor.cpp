#include "TGEditor.hpp"

Texture tex1;
Texture tex2;
Mesh mesh;
Mesh mesh2;
Font arial;
Camera camera;
tg_ui::UIEntity entity = tg_ui::UIEntity({ 1.0f, 0 }, {0.3, 0.15});

int main() {
	Editor editor = Editor();
	std::cout << "Starting Editor" << std::endl;

	tg_ui::UITextureComponent texture = tg_ui::UITextureComponent(&tex2, glm::vec4(2.0f), tg_ui::TOP_RIGHT);
	entity.addComponent(&texture);
	tg_ui::ui_scene_entity.addChildren(&entity);

	tg_model::load("resource\\Chair.fbx", &mesh);
	tg_model::load("resource\\hall.fbx", &mesh2);

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

	tex1 = {
		"resource\\ODST_Helmet.png"
	};
	tex2 = {
		"resource\\test_logo.png"
	};
	createTexture(&tex1);
	createTexture(&tex2);

	createActor(&mesh2)->preScale(0.015, 0.08, 0.005)->prePos(0, -240, 840)->applyPretransform();
	createActor(&mesh)->preRotate(PI / 2, 1.0f, 0, 0)->preScale(0.5, 0.5, 0.5)->applyPretransform();
	//createActor(&mesh);

	createFirstPersonCamera(&camera);

}

void drawloop(IndexBuffer* ibuffer, VertexBuffer* vbuffer)
{

}
