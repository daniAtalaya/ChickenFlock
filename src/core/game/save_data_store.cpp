#include "save_data_store.h"

#include <SDL.h>

#include <algorithm>
#include <fstream>
#include <stdexcept>
#include <system_error>
#include <string>
#include <array>

#ifdef _WIN32
#include <Windows.h>
#endif

SaveDataStore::SaveDataStore() = default;

SaveDataStore::~SaveDataStore() {
	shutdown();
}

void SaveDataStore::shutdown() {
	{
		std::lock_guard lock(mutex);
		if (!writer.joinable()) {
			return;
		}
		stopping = true;
	}
	wakeWriter.notify_one();
	if (writer.joinable()) {
		writer.join();
	}
}

GameProgress SaveDataStore::load() {
	if (loaded || writer.joinable()) {
		throw std::logic_error("Save data can only be loaded once per game session");
	}
	char* preferencePath = SDL_GetPrefPath("Colibri Studios", "Chicken Flock");
	if (preferencePath == nullptr) {
		throw std::runtime_error(std::string("Unable to locate save-data directory: ") + SDL_GetError());
	}
	savePath = std::filesystem::u8path(preferencePath) / "save.dat";
	SDL_free(preferencePath);
	std::filesystem::create_directories(savePath.parent_path());

	GameProgress progress;
	std::ifstream input(savePath);
	if (!input) {
		if (std::filesystem::exists(savePath)) {
			throw std::runtime_error("Unable to open save data at " + savePath.string());
		}
	} else {
		std::string key;
		int version = 0;
		int value = 0;
		std::array<bool, 7> fields{};
		while (input >> key >> value) {
			std::size_t field = 0;
			if (key == "version") {
				field = 0;
				version = value;
			} else if (key == "rupees") {
				field = 1;
				progress.rupees = value;
			} else if (key == "gamesPlayed") {
				field = 2;
				progress.gamesPlayed = value;
			} else if (key == "brownChickenUnlocked") {
				field = 3;
				if (value != 0 && value != 1) throw std::runtime_error("Invalid chicken unlock flag in save data");
				progress.brownChickenUnlocked = value == 1;
			} else if (key == "blueChickenUnlocked") {
				field = 4;
				if (value != 0 && value != 1) throw std::runtime_error("Invalid chicken unlock flag in save data");
				progress.blueChickenUnlocked = value == 1;
			} else if (key == "darkChickenUnlocked") {
				field = 5;
				if (value != 0 && value != 1) throw std::runtime_error("Invalid chicken unlock flag in save data");
				progress.darkChickenUnlocked = value == 1;
			} else if (key == "goldenChickenUnlocked") {
				field = 6;
				if (value != 0 && value != 1) throw std::runtime_error("Invalid chicken unlock flag in save data");
				progress.goldenChickenUnlocked = value == 1;
			} else {
				throw std::runtime_error("Unknown save-data field: " + key);
			}
			if (fields[field]) {
				throw std::runtime_error("Duplicate save-data field: " + key);
			}
			fields[field] = true;
		}
		if (!input.eof()
			|| !std::all_of(fields.begin(), fields.end(), [](const bool present) { return present; })
			|| version != 1 || progress.rupees < 0 || progress.gamesPlayed < 0) {
			throw std::runtime_error("Save data is malformed or uses an unsupported version: " + savePath.string());
		}
	}

	latestRequested = progress;
	writer = std::thread(&SaveDataStore::runWriter, this);
	loaded = true;
	return progress;
}

void SaveDataStore::requestSave(const GameProgress& progress) {
	{
		std::lock_guard lock(mutex);
		if (!loaded || stopping) {
			throw std::logic_error("Save data must be loaded before requesting a save");
		}
		if (progress == latestRequested) {
			return;
		}
		latestRequested = progress;
		pendingSave = progress;
	}
	wakeWriter.notify_one();
}

void SaveDataStore::runWriter() {
	for (;;) {
		GameProgress progress;
		{
			std::unique_lock lock(mutex);
			wakeWriter.wait(lock, [this] { return stopping || pendingSave.has_value(); });
			if (!pendingSave.has_value()) {
				if (stopping) return;
				continue;
			}
			progress = *pendingSave;
			pendingSave.reset();
		}
		try {
			writeAtomically(progress);
		} catch (const std::exception& error) {
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Could not persist save data: %s", error.what());
		}
	}
}

void SaveDataStore::writeAtomically(const GameProgress& progress) const {
	const std::filesystem::path temporaryPath = savePath.string() + ".tmp";
	{
		std::ofstream output(temporaryPath, std::ios::trunc);
		if (!output) {
			throw std::runtime_error("Unable to create save data at " + temporaryPath.string());
		}
		output << "version 1\n"
			<< "rupees " << progress.rupees << '\n'
			<< "gamesPlayed " << progress.gamesPlayed << '\n'
			<< "brownChickenUnlocked " << progress.brownChickenUnlocked << '\n'
			<< "blueChickenUnlocked " << progress.blueChickenUnlocked << '\n'
			<< "darkChickenUnlocked " << progress.darkChickenUnlocked << '\n'
			<< "goldenChickenUnlocked " << progress.goldenChickenUnlocked << '\n';
		output.flush();
		output.close();
		if (!output) {
			throw std::runtime_error("Unable to write save data at " + temporaryPath.string());
		}
	}
#ifdef _WIN32
	if (!MoveFileExW(temporaryPath.c_str(), savePath.c_str(),
		MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH)) {
		throw std::system_error(static_cast<int>(GetLastError()), std::system_category(),
			"Unable to replace save data at " + savePath.string());
	}
#else
	std::filesystem::rename(temporaryPath, savePath);
#endif
}
