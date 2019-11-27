#include "TGEditor.hpp"
#include <gamecontent/Light.hpp>
#include <resources/ShaderPipes.hpp>
#include <pipeline/buffer/Texturebuffer.hpp>

using namespace tge::gmc;
using namespace tge::tex;

tge::gmc::TopDownCamera topdown = tge::gmc::TopDownCamera{ 0, 0, 800, 600 };

int main(int argc, char** args) {
	std::cout << "Starting Editor" << std::endl;

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

	uint32_t actorIdx[] = { 0, 1, 2, 0, 2, 3};
	float actorVertex[] = { -0.5, -0.5, 0, 0, 0.5, -0.5, 1, 0, 0.5, 0.5, 1, 1, -0.5, 0.5, 0, 1};

	ActorInputInfo actorInputs;
	actorInputs.localTransform = {
		1, 0, 0, -1,
		0, 1, 0, 1,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	actorInputs.indices = actorIdx;
	actorInputs.vertices = (uint8_t*) actorVertex;
	actorInputs.material = 0;
	actorInputs.indexCount = 6;
	actorInputs.vertexCount = 4;

	createActor(&actorInputs, 1);

	startTGEngine();
	std::cout << "Clean exit! Bye :wave:!" << std::endl;
	return 0;
}