#pragma once

#include "game_progress.h"

#include <condition_variable>
#include <filesystem>
#include <mutex>
#include <optional>
#include <thread>

class SaveDataStore {
public:
	SaveDataStore();
	SaveDataStore(const SaveDataStore&) = delete;
	SaveDataStore& operator=(const SaveDataStore&) = delete;
	~SaveDataStore();

	GameProgress load();
	void requestSave(const GameProgress& progress);
	void shutdown();

private:
	void runWriter();
	void writeAtomically(const GameProgress& progress) const;

	std::filesystem::path savePath;
	std::mutex mutex;
	std::condition_variable wakeWriter;
	std::optional<GameProgress> pendingSave;
	GameProgress latestRequested;
	std::thread writer;
	bool stopping = false;
	bool loaded = false;
};
