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

	Sampler sampler;
	SamplerBindingInfo bindinginfo;
	createSampler(samplerInputInfo, &sampler, &bindinginfo);
	bindSampler(bindinginfo, 0);
	bindSampler(bindinginfo, 1);

	outtex[1].data = new uint8_t[4] { 255, 255, 255, 255};
	outtex[1].x = 1;
	outtex[1].y = 1;
	outtex[1].comp = 1;

	Texture out[2];
	TextureBindingInfo textureBindingInfos[2048];
	createTextures(outtex, 2, out, textureBindingInfos);
	bindTextures(textureBindingInfos, 2048, 0);
	bindTextures(textureBindingInfos, 2048, 1);

	createdMaterials = new Material[1];
	createdMaterials[0].color = glm::vec4(1);
	createdMaterials[0].diffuseTexture = 0;

	uint32_t actorIdx[] = { 0, 1, 2, 0, 2, 3};
	float actorVertex[] = { -0.5, -0.5, 0, 0, 0.5, -0.5, 1, 0, 0.5, 0.5, 1, 1, -0.5, 0.5, 0, 1};

	ActorInputInfo actorInputs[2];
	actorInputs[0].pProperties.layer = 0;
	actorInputs[0].pProperties.material = 0;
	actorInputs[0].pProperties.localTransform = {
		1, 0, 0, -1,
		0, 1, 0, 1,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	actorInputs[0].indices = actorIdx;
	actorInputs[0].vertices = (uint8_t*) actorVertex;
	actorInputs[0].indexCount = 6;
	actorInputs[0].vertexCount = 4;

	actorInputs[1].pProperties.layer = 1;
	actorInputs[1].pProperties.material = 0;
	actorInputs[1].pProperties.localTransform = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	actorInputs[1].indices = actorIdx;
	actorInputs[1].vertices = nullptr;
	actorInputs[1].indexCount = 6;
	actorInputs[1].vertexCount = 0;

	createActor(actorInputs, 2);

	startTGEngine();
	std::cout << "Clean exit! Bye :wave:!" << std::endl;
	return 0;
}