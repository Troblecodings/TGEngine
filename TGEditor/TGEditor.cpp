#include "TGEditor.hpp"
#include <io/Font.hpp>
#include <gamecontent/Light.hpp>
#include <model/ModelLoader.hpp>

tge::gmc::Mesh mesh;
tge::gmc::Mesh mesh2;
tge::gmc::Camera camera;
tge::gmc::Mesh map;
UIEntity entity = UIEntity(TOP_LEFT, { 0.3, 0.15 });

int main() {
	Editor editor = Editor();

	initEngine(&editor.main_window);
	tge::gmc::Actor actor2;
	tge::mdl::loadGltf("resource\\glTF-Sample-Models\\2.0\\Cube\\glTF\\Cube.gltf", &actor2);
	tge::gmc::actors.push_back(&actor2);

	std::cout << "Starting Editor" << std::endl;
	tge::fnt::Font arial = tge::fnt::Font("resource\\arial.ttf", 40);

	tge::gmc::LightActor light = tge::gmc::LightActor(glm::vec3(0.6, 0.2, 0.5), glm::vec3(10, 0, 0));
	tge::gmc::lights.push_back(&light);

	tge::gmc::LightActor light2 = tge::gmc::LightActor(glm::vec3(0.2, 0.2, 0.5), glm::vec3(3, 0, 0));
	tge::gmc::lights.push_back(&light2);

	tge::gmc::LightActor light3 = tge::gmc::LightActor(glm::vec3(0.5, 0.2, 1.0), glm::vec3(-10, 0, 0));
	tge::gmc::lights.push_back(&light3);


	Texture texture1 = Texture("resource\\test_logo.png");
	//Texture texture2 = Texture("resource\\ODST_Helmet.png");

	UIEntity entity2 = UIEntity(CENTER, glm::vec2(0.1, 0.1));
	entity2.setOffset(CENTER);
	entity.addChildren(&entity2);

	entity.setOffset(CENTER);

	UITextureComponent textureComponent1 = UITextureComponent(&texture1, glm::vec4(1.0f));
	entity.addDrawable(&textureComponent1);

	//UITextureComponent textureComponent2 = UITextureComponent(&texture2, glm::vec4(1.0f));
	//entity2.addDrawable(&textureComponent2);

	UITextComponent text = UITextComponent(&arial, "Test", glm::vec4(1.0f));
	entity.addDrawable(&text);


	ui_scene_entity.addChildren(&entity);

	//tg_model::load("resource\\map2.fbx", &map);
	//tg_model::load("resource\\Chair.fbx", &mesh);
	//tg_model::load("resource\\hall.fbx", &mesh2);

	tge::gmc::Actor actor;
	actor.mesh = &map;
	actor.preRotate(PI / 2, 1, 0, 0)->preScale(0.25, 0.25, 0.25)->prePos(0, -10, 0)->applyPretransform();
	tge::gmc::actors.push_back(&actor);
	tge::gmc::createFirstPersonCamera(&camera);

	startTGEngine(&editor.main_window);
	std::cout << "Clean exit! Bye :wave:!" << std::endl;
	return 0;
}

void init() {
	//createActor(&mesh2)->preScale(0.015, 0.08, 0.005)->prePos(0, -240, 840)->applyPretransform();
	//createActor(&mesh)->preRotate(PI / 2, 1.0f, 0, 0)->preScale(0.5, 0.5, 0.5)->applyPretransform();
	//createActor(&mesh);

}

void drawloop(IndexBuffer* ibuffer, VertexBuffer* vbuffer) {

}
