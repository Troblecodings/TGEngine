#include "Font.hpp"

namespace tge::fnt {

	std::vector<tge::fnt::Font> fonts;
	std::vector<tge::buf::BufferObject> fontBufferObjects;

	void destroyFontresources() {
		tge::buf::destroyBuffers(fontBufferObjects.data(), fontBufferObjects.size());
	}

	void destroyStrings(const uint32_t destroy) {
		auto fontbegin = fontBufferObjects.begin() + (destroy - LAYER_ID_OFFSET);
		for (auto it = fontbegin; fontbegin + 2 != it; it++) {
			vkDestroyBuffer(device, it->buffer, nullptr);
			vkFreeMemory(device, it->memory, nullptr);
			it->memory = VK_NULL_HANDLE;
		}

		auto startitProperties = std::find_if(tge::gmc::actorProperties.begin(), tge::gmc::actorProperties.end(), [&](tge::gmc::ActorProperties prop) { return prop.layer == destroy; });

#ifdef DEBUG
		if (startitProperties == tge::gmc::actorProperties.end()) {
			OUT_LV_DEBUG("Couldn't find layerid in properties");
			return;
		}
#endif // DEBUG

		auto enditProperties = std::find_if(startitProperties, tge::gmc::actorProperties.end(), [&](tge::gmc::ActorProperties prop) { return prop.layer != destroy; });
		auto startitDescriptor = tge::gmc::actorDescriptor.begin() + std::distance(tge::gmc::actorProperties.begin(), startitProperties);
		auto enditDescriptor = startitDescriptor + std::distance(startitProperties, enditProperties);

		tge::gmc::actorProperties.erase(startitProperties, enditProperties);
		tge::gmc::actorDescriptor.erase(startitDescriptor, enditDescriptor);
	}

}