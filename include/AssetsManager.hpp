#pragma once
#include <cstdint>
#include <glm/vec2.hpp>
#include <string_view>
#include <unordered_map>
#include <vector>

class AssetsManager
{
	public:
		static void LoadImage(const std::string_view& ImagePath, uint32_t& OutTextureID, glm::vec2& OutImageSize);

	private:
		inline static std::unordered_map<std::string_view, std::pair<uint32_t, glm::vec2>> TextureIDs;
};