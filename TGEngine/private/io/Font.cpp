#include "../../public/io/Font.hpp"

namespace tge::fnt {

	std::vector<tge::fnt::Font> fonts;
	std::vector<tge::buf::BufferObject> fontBufferObjects;

	void destroyFontresources() {
		tge::buf::destroyBuffers(fontBufferObjects.data(), (uint32_t)fontBufferObjects.size());
	}

	void destroyStrings(const uint32_t destroy) {
		const auto offset = destroy - LAYER_ID_OFFSET;
		const auto offsetend = offset + 1;
#ifdef DEBUG
		if (offsetend >= fontBufferObjects.size()) {
			TGE_CRASH("Odd destroy ID (greater then size)", TG_ERR_DB_NULLPTR)
		}
#endif // DEBUG
		tge::buf::BufferObject buffer = fontBufferObjects[offset];
#ifdef DEBUG
		if (buffer.memory == VK_NULL_HANDLE) {
			TGE_CRASH("Text already destroyed!", TG_ERR_DB_NULLPTR)
		}
#endif // DEBUG
		vkDestroyBuffer(device, buffer.buffer, nullptr);
		vkFreeMemory(device, buffer.memory, nullptr);
		fontBufferObjects[offset].memory = VK_NULL_HANDLE;

		buffer = fontBufferObjects[offsetend];
#ifdef DEBUG
		if (buffer.memory == VK_NULL_HANDLE) {
			TGE_CRASH("Text already destroyed!", TG_ERR_DB_NULLPTR)
		}
#endif // DEBUG
		vkDestroyBuffer(device, buffer.buffer, nullptr);
		vkFreeMemory(device, buffer.memory, nullptr);
		fontBufferObjects[offsetend].memory = VK_NULL_HANDLE;

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