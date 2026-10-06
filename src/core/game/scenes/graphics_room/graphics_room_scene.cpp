#include "graphics_room_scene_factory.h"
#include "asset_catalog.h"
#include "game/game_scene_utils.h"
#include "resources/asset_path.h"

#include <algorithm>
#include <cctype>
#include <string>
#include <utility>
#include <vector>

namespace {
	constexpr SDL_Color paper{ 239, 232, 203, 255 };
	constexpr SDL_Color cream{ 255, 250, 231, 255 };
	constexpr SDL_Color ink{ 44, 67, 53, 255 };
	constexpr SDL_Color leaf{ 78, 100, 74, 255 };
	constexpr SDL_Color border{ 194, 174, 125, 255 };

	std::string lowercase(std::string value) {
		std::transform(value.begin(), value.end(), value.begin(), [](const unsigned char character) {
			return static_cast<char>(std::tolower(character));
		});
		return value;
	}

	void drawArrow(SDL_Renderer* renderer, const SDL_Rect& area, const bool pointsRight) {
		const int centerX = area.x + area.w / 2;
		const int centerY = area.y + area.h / 2;
		const int direction = pointsRight ? 1 : -1;
		SDL_SetRenderDrawColor(renderer, 55, 92, 70, 255);
		SDL_RenderDrawLine(renderer, centerX - 8 * direction, centerY - 15,
			centerX + 8 * direction, centerY);
		SDL_RenderDrawLine(renderer, centerX + 8 * direction, centerY,
			centerX - 8 * direction, centerY + 15);
		SDL_RenderDrawLine(renderer, centerX - 8 * direction, centerY - 15,
			centerX - 8 * direction, centerY + 15);
	}

	void drawTab(SDL_Renderer* renderer, const SDL_Rect& area, const std::string& label,
		const bool selected) {
		SDL_SetRenderDrawColor(renderer, selected ? 255 : 218, selected ? 250 : 224,
			selected ? 231 : 190, 255);
		SDL_RenderFillRect(renderer, &area);
		SDL_SetRenderDrawColor(renderer, border.r, border.g, border.b, border.a);
		SDL_RenderDrawRect(renderer, &area);
		drawPixelText(renderer, label,
			area.x + (area.w - pixelTextWidth(label, 2)) / 2, area.y + 17, 2, ink);
	}

	bool pointIn(const SDL_Point& point, const SDL_Rect& area) {
		return SDL_PointInRect(&point, &area) == SDL_TRUE;
	}
}

class GraphicsRoomScene final : public GameScene {
	GraphicsRoomSceneContext context;
	AssetCatalog catalog;
	AssetCatalogResult catalogResult;
	std::vector<std::size_t> visibleResources;
	SDL_Texture* previewTexture = nullptr;
	Mix_Music* previewMusic = nullptr;
	std::string previewPath;
	std::string query;
	std::string errorMessage;
	std::string catalogError;
	Uint32 queryUpdatedAt = 0;
	std::size_t selectedImage = 0;
	std::size_t selectedAudio = 0;
	std::size_t cachedImageIndex = static_cast<std::size_t>(-1);
	bool catalogReady = false;
	bool scanStarted = false;
	bool audioTab = false;
	bool searchFocused = false;
	bool suppressSlashTextInput = false;
	bool searchPending = false;

	Button backButton;
	SDL_Rect imageTab{ 320, 155, 150, 50 };
	SDL_Rect audioTabButton{ 490, 155, 150, 50 };
	SDL_Rect previousButton{ 92, 430, 72, 72 };
	SDL_Rect nextButton{ 796, 430, 72, 72 };
	SDL_Rect searchBox{ 205, 695, 550, 52 };
	SDL_Rect clearSearchButton{ 707, 700, 42, 42 };
	SDL_Rect previousPageButton{ 245, 778, 180, 48 };
	SDL_Rect nextPageButton{ 535, 778, 180, 48 };
	SDL_Rect playButton{ 430, 725, 100, 48 };

public:
	explicit GraphicsRoomScene(GraphicsRoomSceneContext context) : context(std::move(context)) {
		backButton.img = this->context.assets.images.get("back");
		assignRect(backButton.dstRect, { 24, 24, 72, 72 });
	}

	~GraphicsRoomScene() override {
		stopAudio();
		if (previewTexture != nullptr) SDL_DestroyTexture(previewTexture);
	}

	Escena id() const override { return GRAPHICS_ROOM; }

	void enter(Escena) override {
		SDL_StartTextInput();
		if (!scanStarted) {
			catalog.start(std::filesystem::u8path(assetPath("")));
			scanStarted = true;
		}
	}

	void exit(Escena) override {
		SDL_StopTextInput();
		stopAudio();
	}

	void update() override {
		if (previewMusic != nullptr && !Mix_PlayingMusic()) {
			Mix_FreeMusic(previewMusic);
			previewMusic = nullptr;
		}
		if (!catalogReady && catalog.ready()) {
			catalogResult = catalog.take();
			catalogReady = true;
			catalogError = catalogResult.error;
			searchPending = false;
			rebuildVisibleResources();
		}
		if (searchPending && SDL_GetTicks() - queryUpdatedAt >= 140) {
			applyPendingSearch();
		}
	}

	void handleInput(const SDL_Event& event) override {
		if (event.type == SDL_TEXTINPUT) {
			if (searchFocused && !audioTab) {
				if (suppressSlashTextInput) {
					suppressSlashTextInput = false;
					if (event.text.text[0] == '/') return;
				}
				query.append(event.text.text);
				scheduleSearch();
			}
			return;
		}
		if (event.type != SDL_KEYDOWN || event.key.repeat) return;

		const SDL_Keycode key = event.key.keysym.sym;
		if (key == SDLK_TAB) {
			switchTab();
			return;
		}
		if (searchFocused && !audioTab) {
			if (key == SDLK_BACKSPACE && !query.empty()) {
				eraseLastCharacter();
				scheduleSearch();
				return;
			}
			if (key == SDLK_RETURN || key == SDLK_ESCAPE) {
				applyPendingSearch();
				searchFocused = false;
				suppressSlashTextInput = false;
				return;
			}
			return;
		}
		if (key == SDLK_SLASH) {
			if (!audioTab && !searchFocused) {
				searchFocused = true;
				suppressSlashTextInput = true;
			}
			return;
		}

		switch (key) {
		case SDLK_LEFT: moveSelection(-1); break;
		case SDLK_RIGHT: moveSelection(1); break;
		case SDLK_PAGEUP: moveSelection(-10); break;
		case SDLK_PAGEDOWN: moveSelection(10); break;
		case SDLK_HOME: selectFirst(); break;
		case SDLK_END: selectLast(); break;
		case SDLK_SPACE:
			if (audioTab) toggleAudio();
			break;
		case SDLK_RETURN:
			if (audioTab) toggleAudio();
			else switchTab();
			break;
		default: break;
		}
	}

	void handleClick(const SDL_Point& position) override {
		if (searchPending && !pointIn(position, searchBox)) {
			applyPendingSearch();
		}
		if (backButton.isClicked(position)) {
			context.changeScene(MENU);
		} else if (pointIn(position, imageTab)) {
			setTab(false);
		} else if (pointIn(position, audioTabButton)) {
			setTab(true);
		} else if (pointIn(position, previousButton)) {
			searchFocused = false;
			moveSelection(-1);
		} else if (pointIn(position, nextButton)) {
			searchFocused = false;
			moveSelection(1);
		} else if (pointIn(position, previousPageButton)) {
			searchFocused = false;
			moveSelection(-10);
		} else if (pointIn(position, nextPageButton)) {
			searchFocused = false;
			moveSelection(10);
		} else if (audioTab && pointIn(position, playButton)) {
			toggleAudio();
		} else if (!audioTab && pointIn(position, clearSearchButton)) {
			searchFocused = true;
			query.clear();
			selectedImage = 0;
			searchPending = false;
			rebuildVisibleResources();
		} else if (!audioTab && pointIn(position, searchBox)) {
			searchFocused = true;
		} else {
			searchFocused = false;
		}
	}

	void render(SDL_Renderer* renderer, const bool showHitboxes) override {
		SDL_SetRenderDrawColor(renderer, paper.r, paper.g, paper.b, paper.a);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 218, 224, 190, 255);
		const SDL_Rect topBand{ 0, 0, WINDOW_W, 150 };
		SDL_RenderFillRect(renderer, &topBand);
		SDL_SetRenderDrawColor(renderer, 107, 137, 95, 255);
		SDL_RenderDrawLine(renderer, 0, 150, WINDOW_W, 150);

		backButton.draw(renderer, showHitboxes);
		const std::string title = "FLOCK TROUBLESHOOTING ROOM";
		drawPixelText(renderer, title, (WINDOW_W - pixelTextWidth(title, 3)) / 2,
			45, 3, ink);
		drawTab(renderer, imageTab, "IMAGES", !audioTab);
		drawTab(renderer, audioTabButton, "AUDIO", audioTab);

		if (!catalogReady) {
			drawCentered(renderer, "SCANNING GAME ASSETS", 420, leaf);
			drawCentered(renderer, "THIS ONLY HAPPENS IN THE BACKGROUND", 455, leaf, 2);
			return;
		}
		if (!catalogError.empty()) {
			SDL_Rect errorArea{ 130, 340, 700, 220 };
			drawWrappedPixelText(renderer, "CATALOG ERROR: " + catalogError,
				errorArea, 2, { 150, 45, 35, 255 });
			return;
		}

		const SDL_Rect card{ 170, 230, 620, 435 };
		SDL_SetRenderDrawColor(renderer, cream.r, cream.g, cream.b, cream.a);
		SDL_RenderFillRect(renderer, &card);
		SDL_SetRenderDrawColor(renderer, border.r, border.g, border.b, border.a);
		SDL_RenderDrawRect(renderer, &card);

		if (audioTab) {
			renderAudio(renderer);
		} else {
			renderImage(renderer);
		}
		renderNavigation(renderer);
	}

private:
	void drawCentered(SDL_Renderer* renderer, const std::string& text, const int y,
		const SDL_Color color, const int scale = 3) const {
		drawPixelText(renderer, text, (WINDOW_W - pixelTextWidth(text, scale)) / 2, y, scale, color);
	}

	std::size_t& selectedIndex() {
		return audioTab ? selectedAudio : selectedImage;
	}

	const CatalogResource* selectedResource() const {
		if (audioTab) {
			return selectedAudio < catalogResult.audio.size()
				? &catalogResult.audio[selectedAudio] : nullptr;
		}
		return !visibleResources.empty() && selectedImage < visibleResources.size()
			? &catalogResult.images[visibleResources[selectedImage]] : nullptr;
	}

	void rebuildVisibleResources() {
		visibleResources.clear();
		const std::string needle = lowercase(query);
		for (std::size_t index = 0; index < catalogResult.images.size(); ++index) {
			if (needle.empty() || lowercase(catalogResult.images[index].relativePath.generic_u8string()).find(needle)
				!= std::string::npos) {
				visibleResources.push_back(index);
			}
		}
		if (selectedImage >= visibleResources.size()) selectedImage = 0;
		cachedImageIndex = static_cast<std::size_t>(-1);
		if (previewTexture != nullptr) {
			SDL_DestroyTexture(previewTexture);
			previewTexture = nullptr;
		}
	}

	void scheduleSearch() {
		queryUpdatedAt = SDL_GetTicks();
		searchPending = true;
	}

	void applyPendingSearch() {
		if (!searchPending) return;
		searchPending = false;
		selectedImage = 0;
		rebuildVisibleResources();
	}

	void setTab(const bool toAudio) {
		if (audioTab == toAudio) return;
		applyPendingSearch();
		audioTab = toAudio;
		searchFocused = false;
		errorMessage.clear();
		stopAudio();
	}

	void switchTab() {
		setTab(!audioTab);
	}

	void moveSelection(const int offset) {
		auto& index = selectedIndex();
		const std::size_t count = audioTab ? catalogResult.audio.size() : visibleResources.size();
		if (count == 0) return;
		const int next = static_cast<int>(index) + offset;
		index = static_cast<std::size_t>((next % static_cast<int>(count) + static_cast<int>(count))
			% static_cast<int>(count));
		errorMessage.clear();
		if (audioTab) stopAudio();
	}

	void selectFirst() {
		selectedIndex() = 0;
		errorMessage.clear();
		if (audioTab) stopAudio();
	}

	void selectLast() {
		const std::size_t count = audioTab ? catalogResult.audio.size() : visibleResources.size();
		if (count > 0) selectedIndex() = count - 1;
		errorMessage.clear();
		if (audioTab) stopAudio();
	}

	void renderImage(SDL_Renderer* renderer) {
		const SDL_Rect artwork{ 215, 245, 530, 290 };
		SDL_SetRenderDrawColor(renderer, 225, 232, 211, 255);
		SDL_RenderFillRect(renderer, &artwork);
		SDL_SetRenderDrawColor(renderer, 191, 207, 171, 255);
		SDL_RenderDrawRect(renderer, &artwork);

		const CatalogResource* resource = selectedResource();
		if (resource == nullptr) {
			drawCentered(renderer, "NO IMAGES MATCH THIS SEARCH", 390, leaf, 2);
		} else {
			loadPreview(*resource);
			if (previewTexture != nullptr) {
				int width = 0;
				int height = 0;
				SDL_QueryTexture(previewTexture, nullptr, nullptr, &width, &height);
				if (width > 0 && height > 0) {
					const double fit = std::min(
						static_cast<double>(artwork.w - 32) / width,
						static_cast<double>(artwork.h - 32) / height);
					const int fittedWidth = std::max(1, static_cast<int>(width * fit));
					const int fittedHeight = std::max(1, static_cast<int>(height * fit));
					const SDL_Rect destination{
						artwork.x + (artwork.w - fittedWidth) / 2,
						artwork.y + (artwork.h - fittedHeight) / 2,
						fittedWidth,
						fittedHeight
					};
					SDL_RenderCopy(renderer, previewTexture, nullptr, &destination);
				}
			} else if (!errorMessage.empty()) {
				drawWrappedPixelText(renderer, errorMessage, { 235, 370, 490, 90 }, 2,
					{ 150, 45, 35, 255 });
			}

			const std::string path = resource->relativePath.generic_u8string();
			drawWrappedPixelText(renderer, path, { 225, 545, 510, 38 }, 2, ink);
			const auto description = catalogResult.descriptions.find(path);
			const std::string details = description == catalogResult.descriptions.end()
				? "NO CURATOR NOTE FOR THIS RESOURCE." : description->second;
			drawWrappedPixelText(renderer, details, { 225, 590, 510, 60 }, 2, leaf);
		}
		renderSearch(renderer);
	}

	void renderAudio(SDL_Renderer* renderer) {
		const SDL_Rect waveform{ 240, 290, 480, 220 };
		SDL_SetRenderDrawColor(renderer, 225, 232, 211, 255);
		SDL_RenderFillRect(renderer, &waveform);
		SDL_SetRenderDrawColor(renderer, 191, 207, 171, 255);
		SDL_RenderDrawRect(renderer, &waveform);

		for (int bar = 0; bar < 40; ++bar) {
			const int height = 22 + ((bar * 29 + 17) % 125);
			const SDL_Rect waveBar{ waveform.x + 14 + bar * 11,
				waveform.y + (waveform.h - height) / 2, 4, height };
			SDL_SetRenderDrawColor(renderer, 107, 137, 95, 255);
			SDL_RenderFillRect(renderer, &waveBar);
		}

		const CatalogResource* resource = selectedResource();
		if (resource == nullptr) {
			drawCentered(renderer, "NO AUDIO RESOURCES FOUND", 555, leaf, 2);
		} else {
			const std::string path = resource->relativePath.generic_u8string();
			drawWrappedPixelText(renderer, path, { 220, 540, 520, 56 }, 2, ink);
			drawCentered(renderer, previewMusic != nullptr && Mix_PlayingMusic()
				? "PLAYING" : "READY", 615, leaf, 2);
			const std::string position = std::to_string(selectedAudio + 1)
				+ "/" + std::to_string(catalogResult.audio.size()) + " AUDIO";
			drawPixelText(renderer, position,
				(WINDOW_W - pixelTextWidth(position, 2)) / 2, 648, 2, leaf);
		}

		SDL_SetRenderDrawColor(renderer, cream.r, cream.g, cream.b, cream.a);
		SDL_RenderFillRect(renderer, &playButton);
		SDL_SetRenderDrawColor(renderer, border.r, border.g, border.b, border.a);
		SDL_RenderDrawRect(renderer, &playButton);
		const std::string label = previewMusic != nullptr && Mix_PlayingMusic() ? "STOP" : "PLAY";
		drawPixelText(renderer, label,
			playButton.x + (playButton.w - pixelTextWidth(label, 2)) / 2,
			playButton.y + (playButton.h - 14) / 2, 2, ink);
		if (!errorMessage.empty()) {
			drawWrappedPixelText(renderer, errorMessage, { 220, 670, 520, 48 }, 2,
				{ 150, 45, 35, 255 });
		}
	}

	void renderSearch(SDL_Renderer* renderer) {
		SDL_SetRenderDrawColor(renderer, searchFocused ? 255 : 249, searchFocused ? 250 : 244,
			searchFocused ? 231 : 224, 255);
		SDL_RenderFillRect(renderer, &searchBox);
		SDL_SetRenderDrawColor(renderer, searchFocused ? 107 : border.r,
			searchFocused ? 137 : border.g, searchFocused ? 95 : border.b, 255);
		SDL_RenderDrawRect(renderer, &searchBox);
		SDL_Rect textClip{ searchBox.x + 12, searchBox.y + 8,
			searchBox.w - clearSearchButton.w - 28, searchBox.h - 16 };
		SDL_RenderSetClipRect(renderer, &textClip);
		std::string label = query.empty() ? "SEARCH ASSETS" : query;
		constexpr int textScale = 2;
		const int maxCharacters = std::max(1, textClip.w / (6 * textScale) - 1);
		if (label.size() > static_cast<std::size_t>(maxCharacters)) {
			label.erase(0, label.size() - static_cast<std::size_t>(maxCharacters));
		}
		const int textY = searchBox.y + (searchBox.h - 7 * textScale) / 2;
		drawPixelText(renderer, label, textClip.x, textY, textScale,
			query.empty() ? leaf : ink);
		if (searchFocused && (SDL_GetTicks() / 450) % 2 == 0) {
			const int cursorX = textClip.x + pixelTextWidth(label, textScale) + 3;
			SDL_SetRenderDrawColor(renderer, ink.r, ink.g, ink.b, ink.a);
			SDL_RenderDrawLine(renderer, cursorX, textY, cursorX, textY + 7 * textScale);
		}
		SDL_RenderSetClipRect(renderer, nullptr);

		SDL_SetRenderDrawColor(renderer, 255, 239, 190, 255);
		SDL_RenderFillRect(renderer, &clearSearchButton);
		SDL_SetRenderDrawColor(renderer, border.r, border.g, border.b, 255);
		SDL_RenderDrawRect(renderer, &clearSearchButton);
		SDL_SetRenderDrawColor(renderer, leaf.r, leaf.g, leaf.b, leaf.a);
		if (query.empty()) {
			SDL_Rect lens{ clearSearchButton.x + 10, clearSearchButton.y + 9, 17, 17 };
			SDL_RenderDrawRect(renderer, &lens);
			SDL_RenderDrawLine(renderer, clearSearchButton.x + 25, clearSearchButton.y + 25,
				clearSearchButton.x + 32, clearSearchButton.y + 32);
		} else {
			SDL_RenderDrawLine(renderer, clearSearchButton.x + 14,
				clearSearchButton.y + 14, clearSearchButton.x + 28, clearSearchButton.y + 28);
			SDL_RenderDrawLine(renderer, clearSearchButton.x + 28,
				clearSearchButton.y + 14, clearSearchButton.x + 14, clearSearchButton.y + 28);
		}

		const std::string count = searchPending ? "FILTERING..."
			: std::to_string(visibleResources.empty() ? 0 : selectedImage + 1)
				+ "/" + std::to_string(visibleResources.size()) + " IMAGES";
		drawPixelText(renderer, count, (WINDOW_W - pixelTextWidth(count, 2)) / 2, 752, 2, leaf);
	}

	void renderNavigation(SDL_Renderer* renderer) {
		SDL_SetRenderDrawColor(renderer, cream.r, cream.g, cream.b, cream.a);
		SDL_RenderFillRect(renderer, &previousButton);
		SDL_RenderFillRect(renderer, &nextButton);
		SDL_SetRenderDrawColor(renderer, border.r, border.g, border.b, border.a);
		SDL_RenderDrawRect(renderer, &previousButton);
		SDL_RenderDrawRect(renderer, &nextButton);
		drawArrow(renderer, previousButton, false);
		drawArrow(renderer, nextButton, true);
		drawNavigationButton(renderer, previousPageButton, "BACK 10");
		drawNavigationButton(renderer, nextPageButton, "NEXT 10");
		const std::string hint = audioTab
			? "SPACE PLAY   HOME / END JUMP"
			: "ARROWS STEP   PGUP / PGDN JUMP   / SEARCH";
		drawPixelText(renderer, hint,
			(WINDOW_W - pixelTextWidth(hint, 2)) / 2, 842, 2, leaf);
	}

	void drawNavigationButton(SDL_Renderer* renderer, const SDL_Rect& area,
		const std::string& label) const {
		SDL_SetRenderDrawColor(renderer, 255, 250, 231, 255);
		SDL_RenderFillRect(renderer, &area);
		SDL_SetRenderDrawColor(renderer, border.r, border.g, border.b, 255);
		SDL_RenderDrawRect(renderer, &area);
		drawPixelText(renderer, label, area.x + (area.w - pixelTextWidth(label, 2)) / 2,
			area.y + (area.h - 14) / 2, 2, ink);
	}

	void eraseLastCharacter() {
		if (query.empty()) return;
		std::size_t lastCharacter = query.size() - 1;
		while (lastCharacter > 0
			&& (static_cast<unsigned char>(query[lastCharacter]) & 0xC0) == 0x80) {
			--lastCharacter;
		}
		query.erase(lastCharacter);
	}

	void loadPreview(const CatalogResource& resource) {
		const std::size_t actualIndex = visibleResources[selectedImage];
		if (cachedImageIndex == actualIndex) return;
		cachedImageIndex = actualIndex;
		if (previewTexture != nullptr) {
			SDL_DestroyTexture(previewTexture);
			previewTexture = nullptr;
		}
		errorMessage.clear();
		const std::string path = resource.absolutePath.u8string();
		previewTexture = IMG_LoadTexture(context.renderer, path.c_str());
		if (previewTexture == nullptr) {
			errorMessage = SDL_GetError();
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to display gallery image %s: %s",
				path.c_str(), errorMessage.c_str());
		}
	}

	void toggleAudio() {
		const CatalogResource* resource = selectedResource();
		if (resource == nullptr) return;
		if (previewMusic != nullptr) {
			stopAudio();
			return;
		}
		errorMessage.clear();
		const std::string path = resource->absolutePath.u8string();
		previewMusic = Mix_LoadMUS(path.c_str());
		if (previewMusic == nullptr) {
			errorMessage = Mix_GetError();
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to load gallery audio %s: %s",
				path.c_str(), errorMessage.c_str());
			return;
		}
		if (Mix_PlayMusic(previewMusic, 0) != 0) {
			errorMessage = Mix_GetError();
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Unable to play gallery audio %s: %s",
				path.c_str(), errorMessage.c_str());
			Mix_FreeMusic(previewMusic);
			previewMusic = nullptr;
		}
	}

	void stopAudio() {
		if (previewMusic == nullptr) return;
		if (Mix_PlayingMusic()) Mix_HaltMusic();
		Mix_FreeMusic(previewMusic);
		previewMusic = nullptr;
	}
};

std::unique_ptr<GameScene> createGraphicsRoomScene(GraphicsRoomSceneContext context) {
	return std::make_unique<GraphicsRoomScene>(std::move(context));
}
