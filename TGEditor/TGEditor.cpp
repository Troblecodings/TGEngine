#include "TGEditor.hpp"
#include <gamecontent/Light.hpp>
#include <model/ModelLoader.hpp>
#include <resources/ShaderPipes.hpp>

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

	Texture textex = Texture("resource\\grass.png");
	vlibSamplerCreateInfo.magFilter = VK_FILTER_NEAREST;
	vlibSamplerCreateInfo.minFilter = VK_FILTER_NEAREST;
	vlibSamplerCreateInfo.anisotropyEnable = FALSE;
	vlibSamplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
	vlibSamplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	vlibSamplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	createSampler(&textex.sampler);
	textex.miplevels = 1;
	textex.initTexture();
	tge::tex::textures.push_back(&textex);

	Material mat = Material(&Tex2DPipe);
	mat.doubleSided = true;
	mat.texture = &textex;
	materiallist.push_back(&mat);

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

	render_offset.push_back({0, 6, 0});

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
