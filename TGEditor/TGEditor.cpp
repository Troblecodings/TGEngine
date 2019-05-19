#include "TGEditor.hpp"

Mesh mesh;
Mesh mesh2;
Camera camera;
tg_ui::UIEntity entity = tg_ui::UIEntity({ 0.0f, 0.0f }, {0.3, 0.15});

int main() {
	Editor editor = Editor();
	std::cout << "Starting Editor" << std::endl;
	tg_font::Font arial = tg_font::Font("resource\\arial.ttf", 40);

	Texture tex1 = Texture("resource\\ODST_Helmet.png");
	Texture tex2 = Texture("resource\\test_logo.png");

	tg_ui::UITextureComponent texture = tg_ui::UITextureComponent(&tex2, glm::vec4(1.0f), tg_ui::TOP_LEFT);

	tg_ui::UITextureComponent texcomp2 = tg_ui::UITextureComponent(&tex1, glm::vec4(1.0f), tg_ui::CENTER);
	tg_ui::UIEntity entity2 = tg_ui::UIEntity(tg_ui::CENTER, glm::vec2(0.1, 0.1));
	entity2.addComponent(&texcomp2);

	entity.addChildren(&entity2);

	tg_ui::UITextComponent text = tg_ui::UITextComponent(&arial, "Test", glm::vec4(1.0f), tg_ui::BOTTOM_LEFT);
	entity.addComponent(&texture);
	entity.addComponent(&text);
	tg_ui::ui_scene_entity.addChildren(&entity);

	tg_model::load("resource\\Chair.fbx", &mesh);
	tg_model::load("resource\\hall.fbx", &mesh2);

	initTGEngine(&editor.main_window, &drawloop, &init);
	std::cout << "Clean exit! Bye :wave:!" << std::endl;
	return 0;
}

void init() {
	createActor(&mesh2)->preScale(0.015, 0.08, 0.005)->prePos(0, -240, 840)->applyPretransform();
	createActor(&mesh)->preRotate(PI / 2, 1.0f, 0, 0)->preScale(0.5, 0.5, 0.5)->applyPretransform();
	//createActor(&mesh);

	createFirstPersonCamera(&camera);

}

void drawloop(IndexBuffer* ibuffer, VertexBuffer* vbuffer)
{

}
