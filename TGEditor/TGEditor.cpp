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
	tge::gmc::createFirstPersonCamera(&camera);

	initEngine(&editor.main_window);
	tge::gmc::Actor actor2;
	tge::mdl::loadGltf("resource\\glTF-Sample-Models\\2.0\\Cube\\glTF\\Cube.gltf", &actor2);
	tge::gmc::actors.push_back(&actor2);

	std::cout << "Starting Editor" << std::endl;

	tge::gmc::LightActor light = tge::gmc::LightActor(glm::vec3(0.2, 0.2, 0.2), glm::vec3(10, 0, 0));
	tge::gmc::lights.push_back(&light);

	tge::gmc::LightActor light2 = tge::gmc::LightActor(glm::vec3(0.2, 0.2, 0.2), glm::vec3(3, 0, 0));
	tge::gmc::lights.push_back(&light2);

	tge::gmc::LightActor light3 = tge::gmc::LightActor(glm::vec3(0.2, 0.2, 0.2), glm::vec3(-10, 0, 0));
	tge::gmc::lights.push_back(&light3);


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
