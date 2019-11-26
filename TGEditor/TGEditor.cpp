#include "TGEditor.hpp"
#include <gamecontent/Light.hpp>
#include <resources/ShaderPipes.hpp>
#include <pipeline/buffer/Texturebuffer.hpp>

using namespace tge::gmc;
using namespace tge::tex;

tge::gmc::Camera camera;
UIEntity entity = UIEntity(TOP_LEFT, { 0.3, 0.15 });
tge::gmc::TopDownCamera topdown = tge::gmc::TopDownCamera{ 0, 0, 800, 600 };

int main(int argc, char** args) {
	initEngine();

	setTopDownCamera(&topdown);
	playercontroller = [](tge::gmc::Input* input) { topdown.positiony += input->y1; topdown.positionx -= input->x1; tge::gmc::setTopDownCamera(&topdown); };

	ResourceDescriptor texture;
	texture.offset = 0;
	texture.size = 10839;

	File resc = open("resource\\Resources.tgr", "rb");

	TextureInputInfo outtex[2];
	loadTextures(resc, &texture, 1, outtex);

	fclose(resc);

	SamplerInputInfo samplerInputInfo;
	samplerInputInfo.uSamplerMode = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInputInfo.vSamplerMode = VK_SAMPLER_ADDRESS_MODE_REPEAT;
	samplerInputInfo.filterMagnification = VK_FILTER_NEAREST;
	samplerInputInfo.filterMignification = VK_FILTER_NEAREST;

	createSampler(samplerInputInfo);

	outtex[1].data = new uint8_t[4] { 255, 255, 255, 255};
	outtex[1].x = 1;
	outtex[1].y = 1;
	outtex[1].comp = 1;

	Texture out[2];
	createTextures(outtex, 2, out);

	createdMaterials = new Material[1];
	createdMaterials[0].color = glm::vec4(1);
	createdMaterials[0].diffuseTexture = 0;

	vertexBuffer.add(glm::vec4(-2, -2, 0, 0))->endVertex();
	vertexBuffer.add(glm::vec4(2, -2, 10, 0))->endVertex();
	vertexBuffer.add(glm::vec4(2, 2, 10, 10))->endVertex();
	vertexBuffer.add(glm::vec4(-2, 2, 0, 10))->endVertex();

	indexBuffer.addIndex(0);
	indexBuffer.addIndex(1);
	indexBuffer.addIndex(2);
	indexBuffer.addIndex(0);
	indexBuffer.addIndex(2);
	indexBuffer.addIndex(3);

	ActorInputInfo actorInputs;
	actorInputs.size = 6;
	actorInputs.offset = 0;
	actorInputs.localTransform = glm::mat4(1);
	createActor(&actorInputs, 1);

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
