#include "asset_catalog.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <system_error>

namespace {
	std::string lowercase(std::string value) {
		std::transform(value.begin(), value.end(), value.begin(), [](const unsigned char character) {
			return static_cast<char>(std::tolower(character));
		});
		return value;
	}

	std::string readJsonString(const std::string& source, std::size_t& position) {
		while (position < source.size() && std::isspace(static_cast<unsigned char>(source[position]))) {
			++position;
		}
		if (position >= source.size() || source[position++] != '"') {
			throw std::runtime_error("Expected a quoted gallery-description string");
		}
		std::string value;
		while (position < source.size()) {
			const char character = source[position++];
			if (character == '"') return value;
			if (character == '\\') {
				if (position >= source.size()) {
					throw std::runtime_error("Incomplete escape in gallery descriptions");
				}
				const char escaped = source[position++];
				if (escaped != '"' && escaped != '\\' && escaped != '/') {
					throw std::runtime_error("Unsupported escape in gallery descriptions");
				}
				value.push_back(escaped);
			} else {
				value.push_back(character);
			}
		}
		throw std::runtime_error("Unterminated gallery-description string");
	}

	std::map<std::string, std::string> loadDescriptions(const std::filesystem::path& path) {
		std::ifstream input(path, std::ios::binary);
		if (!input) {
			throw std::runtime_error("Unable to open gallery descriptions: " + path.string());
		}
		std::ostringstream contents;
		contents << input.rdbuf();
		if (!input.good() && !input.eof()) {
			throw std::runtime_error("Unable to read gallery descriptions: " + path.string());
		}

		const std::string json = contents.str();
		std::size_t position = 0;
		while (position < json.size() && std::isspace(static_cast<unsigned char>(json[position]))) ++position;
		if (position >= json.size() || json[position++] != '{') {
			throw std::runtime_error("Gallery descriptions must be a JSON object");
		}

		std::map<std::string, std::string> descriptions;
		for (;;) {
			while (position < json.size() && std::isspace(static_cast<unsigned char>(json[position]))) ++position;
			if (position < json.size() && json[position] == '}') {
				++position;
				break;
			}
			const std::string key = readJsonString(json, position);
			while (position < json.size() && std::isspace(static_cast<unsigned char>(json[position]))) ++position;
			if (position >= json.size() || json[position++] != ':') {
				throw std::runtime_error("Expected ':' in gallery descriptions");
			}
			const std::string value = readJsonString(json, position);
			if (!descriptions.emplace(key, value).second) {
				throw std::runtime_error("Duplicate key in gallery descriptions: " + key);
			}
			while (position < json.size() && std::isspace(static_cast<unsigned char>(json[position]))) ++position;
			if (position < json.size() && json[position] == ',') {
				++position;
				continue;
			}
			if (position < json.size() && json[position] == '}') {
				++position;
				break;
			}
			throw std::runtime_error("Expected ',' or '}' in gallery descriptions");
		}
		while (position < json.size() && std::isspace(static_cast<unsigned char>(json[position]))) ++position;
		if (position != json.size()) {
			throw std::runtime_error("Unexpected data after gallery descriptions");
		}
		return descriptions;
	}

	bool isImage(const std::string& extension) {
		return extension == ".png" || extension == ".jpg" || extension == ".jpeg"
			|| extension == ".bmp" || extension == ".gif" || extension == ".webp"
			|| extension == ".tif" || extension == ".tiff" || extension == ".tga"
			|| extension == ".ppm" || extension == ".pgm" || extension == ".pbm"
			|| extension == ".pcx" || extension == ".xcf" || extension == ".qoi"
			|| extension == ".xpm";
	}

	bool isAudio(const std::string& extension) {
		return extension == ".wav" || extension == ".ogg" || extension == ".mp3"
			|| extension == ".flac" || extension == ".opus" || extension == ".mid"
			|| extension == ".midi" || extension == ".aif" || extension == ".aiff"
			|| extension == ".voc" || extension == ".mod" || extension == ".xm"
			|| extension == ".it" || extension == ".s3m";
	}

	bool pathLess(const CatalogResource& left, const CatalogResource& right) {
		return lowercase(left.relativePath.generic_u8string())
			< lowercase(right.relativePath.generic_u8string());
	}
}

AssetCatalog::~AssetCatalog() {
	if (worker.joinable()) worker.join();
}

void AssetCatalog::start(std::filesystem::path assetsRoot) {
	if (worker.joinable() || complete.load()) {
		throw std::logic_error("Asset catalog scan can only be started once");
	}
	worker = std::thread(&AssetCatalog::scan, this, std::move(assetsRoot));
}

bool AssetCatalog::ready() const {
	return complete.load(std::memory_order_acquire);
}

AssetCatalogResult AssetCatalog::take() {
	if (!ready()) {
		throw std::logic_error("Asset catalog results are not ready");
	}
	if (worker.joinable()) worker.join();
	return std::move(result);
}

void AssetCatalog::scan(std::filesystem::path assetsRoot) {
	try {
		AssetCatalogResult scanned;
		scanned.descriptions = loadDescriptions(assetsRoot / "gallery" / "descriptions.json");
		std::error_code error;
		std::filesystem::recursive_directory_iterator iterator(assetsRoot, error);
		const std::filesystem::recursive_directory_iterator end;
		if (error) {
			throw std::filesystem::filesystem_error("Unable to scan game assets", assetsRoot, error);
		}
		for (; iterator != end;) {
			if (error) {
				throw std::filesystem::filesystem_error("Unable to scan game assets", assetsRoot, error);
			}
			const std::filesystem::path path = iterator->path();
			if (iterator->is_regular_file(error)) {
				if (error) throw std::filesystem::filesystem_error("Unable to inspect game asset", path, error);
				const std::filesystem::path relative = std::filesystem::relative(path, assetsRoot, error);
				if (error) {
					throw std::filesystem::filesystem_error("Unable to resolve game asset path", path, error);
				}
				const std::string extension = lowercase(path.extension().u8string());
				if (isImage(extension)) {
					scanned.images.push_back({ CatalogResourceType::Image, relative, path });
				} else if (isAudio(extension)) {
					scanned.audio.push_back({ CatalogResourceType::Audio, relative, path });
				}
			} else if (error) {
				throw std::filesystem::filesystem_error("Unable to inspect game asset", path, error);
			}
			iterator.increment(error);
			if (error) {
				throw std::filesystem::filesystem_error("Unable to scan game assets", assetsRoot, error);
			}
		}
		std::sort(scanned.images.begin(), scanned.images.end(), pathLess);
		std::sort(scanned.audio.begin(), scanned.audio.end(), pathLess);
		if (scanned.images.empty()) {
			throw std::runtime_error("No image assets were found under " + assetsRoot.string());
		}
		result = std::move(scanned);
	} catch (const std::exception& error) {
		result.error = error.what();
	}
	complete.store(true, std::memory_order_release);
}
