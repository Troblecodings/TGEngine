#include "TGEditor.hpp"
#include <gamecontent/Light.hpp>
#include <model/ModelLoader.hpp>

tge::gmc::Mesh mesh;
tge::gmc::Mesh mesh2;
tge::gmc::Camera camera;
tge::gmc::Mesh map;
UIEntity entity = UIEntity(TOP_LEFT, { 0.3, 0.15 });

int main(int argc, char** args) {
	initEngine();
	tge::gmc::Model actor2;

	tge::gmc::TopDownCamera topdown = { 0.5, 0, 800, 600 };
	tge::gmc::setTopDownCamera(&topdown);

	tge::mdl::loadGltf(argc > 1 ? args[1] : "resource\\glTF-Sample-Models\\2.0\\Cube\\glTF\\Cube.gltf", &actor2);
	tge::gmc::models.push_back(&actor2);

	std::cout << "Starting Editor" << std::endl;

	tge::gmc::LightActor light = tge::gmc::LightActor(1, glm::vec3(0.05, 0.05, 0.05), glm::vec3(0, 0, 0));
	tge::gmc::lights.push_back(&light);

	startTGEngine();
	std::cout << "Clean exit! Bye :wave:!" << std::endl;
	return 0;
}

void init() {
}

void drawloop(IndexBuffer* ibuffer, VertexBuffer* vbuffer) {

}
