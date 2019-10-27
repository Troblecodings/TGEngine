#include "TGEditor.hpp"
#include <gamecontent/Light.hpp>
#include <model/ModelLoader.hpp>
#include <resources/ShaderPipes.hpp>
#include <pipeline/buffer/Texturebuffer.hpp>

using namespace tge::gmc;
using namespace tge::tex;

tge::gmc::Mesh mesh;
tge::gmc::Mesh mesh2;
tge::gmc::Camera camera;
tge::gmc::Mesh map;
UIEntity entity = UIEntity(TOP_LEFT, { 0.3, 0.15 });
tge::gmc::TopDownCamera topdown = tge::gmc::TopDownCamera{ 0, 0, 800, 600 };

int main(int argc, char** args) {
	initEngine();
	Model actor2;

	setTopDownCamera(&topdown);
	playercontroller = [](tge::gmc::Input* input) { topdown.positiony += input->y1; topdown.positionx -= input->x1; tge::gmc::setTopDownCamera(&topdown); };

	ResourceDescriptor texture;
	texture.offset = 0;
	texture.size = 665;

	File resc = open("resource\\Resources.tgr", "rb");

	TextureInputInfo outtex;
	loadTextures(resc, &texture, 1, &outtex);

	Texture out;
	createTextures(&outtex, 1, &out);

	fclose(resc);

	vertexBuffer.add(glm::vec4(-4, -4, 0, 0))->add(glm::vec4(1, 1, 1, 1))->endVertex();
	vertexBuffer.add(glm::vec4(4, -4, 16, 0))->add(glm::vec4(1, 1, 1, 1))->endVertex();
	vertexBuffer.add(glm::vec4(4, 4, 16, 16))->add(glm::vec4(1, 1, 1, 1))->endVertex();
	vertexBuffer.add(glm::vec4(-4, 4, 0, 16))->add(glm::vec4(1, 1, 1, 1))->endVertex();

	indexBuffer.addIndex(0);
	indexBuffer.addIndex(1);
	indexBuffer.addIndex(2);
	indexBuffer.addIndex(0);
	indexBuffer.addIndex(2);
	indexBuffer.addIndex(3);

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
