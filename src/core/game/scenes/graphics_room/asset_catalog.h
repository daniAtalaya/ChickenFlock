#pragma once

#include <atomic>
#include <filesystem>
#include <map>
#include <string>
#include <thread>
#include <vector>

enum class CatalogResourceType {
	Image,
	Audio
};

struct CatalogResource {
	CatalogResourceType type;
	std::filesystem::path relativePath;
	std::filesystem::path absolutePath;
};

struct AssetCatalogResult {
	std::vector<CatalogResource> images;
	std::vector<CatalogResource> audio;
	std::map<std::string, std::string> descriptions;
	std::string error;
};

class AssetCatalog {
public:
	AssetCatalog() = default;
	AssetCatalog(const AssetCatalog&) = delete;
	AssetCatalog& operator=(const AssetCatalog&) = delete;
	~AssetCatalog();

	void start(std::filesystem::path assetsRoot);
	bool ready() const;
	AssetCatalogResult take();

private:
	void scan(std::filesystem::path assetsRoot);

	std::thread worker;
	std::atomic<bool> complete{ false };
	AssetCatalogResult result;
};
