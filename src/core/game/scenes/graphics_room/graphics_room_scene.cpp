#include "graphics_room_scene_factory.h"
#include "game/game_scene_utils.h"

#include <algorithm>
#include <array>
#include <string>
#include <utility>

namespace {
	struct Exhibit {
		const char* texture;
		const char* title;
	};

	constexpr std::array<Exhibit, 5> exhibits{{
		{ "link", "PLAYER SPRITE" },
		{ "mascota", "GOLDEN COMPANION" },
		{ "pajaro", "SKY MESSENGER" },
		{ "horda", "THE HORDE" },
		{ "mapa3", "FOREST TRAIL" }
	}};

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
}

class GraphicsRoomScene final : public GameScene {
	GraphicsRoomSceneContext context;
	Button backButton;
	std::size_t selected = 0;
	SDL_Rect previousButton{ 92, 438, 64, 64 };
	SDL_Rect nextButton{ 804, 438, 64, 64 };

public:
	explicit GraphicsRoomScene(GraphicsRoomSceneContext context) : context(std::move(context)) {
		backButton.img = this->context.assets.images.get("back");
		assignRect(backButton.dstRect, { 24, 24, 72, 72 });
	}

	Escena id() const override { return GRAPHICS_ROOM; }

	void handleInput(const SDL_Event& event) override {
		if (event.type != SDL_KEYDOWN || event.key.repeat) return;
		switch (event.key.keysym.sym) {
		case SDLK_LEFT: select(-1); break;
		case SDLK_RIGHT: select(1); break;
		case SDLK_RETURN:
		case SDLK_SPACE:
			context.changeScene(MENU);
			break;
		default: break;
		}
	}

	void handleClick(const SDL_Point& position) override {
		if (backButton.isClicked(position)) {
			context.changeScene(MENU);
		} else if (SDL_PointInRect(&position, &previousButton)) {
			select(-1);
		} else if (SDL_PointInRect(&position, &nextButton)) {
			select(1);
		}
	}

	void render(SDL_Renderer* renderer, const bool showHitboxes) override {
		SDL_SetRenderDrawColor(renderer, 239, 232, 203, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 218, 224, 190, 255);
		const SDL_Rect topBand{ 0, 0, WINDOW_W, 150 };
		SDL_RenderFillRect(renderer, &topBand);
		SDL_SetRenderDrawColor(renderer, 107, 137, 95, 255);
		SDL_RenderDrawLine(renderer, 0, 150, WINDOW_W, 150);

		backButton.draw(renderer, showHitboxes);
		const std::string title = "GRAPHICS ROOM";
		const std::string subtitle = "A LITTLE GALLERY OF THE FLOCK";
		drawPixelText(renderer, title,
			(WINDOW_W - pixelTextWidth(title, 5)) / 2, 52, 5, { 44, 67, 53, 255 });
		drawPixelText(renderer, subtitle,
			(WINDOW_W - pixelTextWidth(subtitle, 2)) / 2, 112, 2, { 78, 100, 74, 255 });

		const SDL_Rect card{ 180, 185, 600, 540 };
		SDL_SetRenderDrawColor(renderer, 255, 250, 231, 255);
		SDL_RenderFillRect(renderer, &card);
		SDL_SetRenderDrawColor(renderer, 194, 174, 125, 255);
		SDL_RenderDrawRect(renderer, &card);
		const SDL_Rect artwork{ 225, 225, 510, 400 };
		SDL_SetRenderDrawColor(renderer, 225, 232, 211, 255);
		SDL_RenderFillRect(renderer, &artwork);
		SDL_SetRenderDrawColor(renderer, 191, 207, 171, 255);
		SDL_RenderDrawRect(renderer, &artwork);

		SDL_Texture* texture = context.assets.images.get(exhibits[selected].texture);
		int imageWidth = 0;
		int imageHeight = 0;
		SDL_QueryTexture(texture, nullptr, nullptr, &imageWidth, &imageHeight);
		if (imageWidth > 0 && imageHeight > 0) {
			const double fit = std::min(
				static_cast<double>(artwork.w - 36) / imageWidth,
				static_cast<double>(artwork.h - 36) / imageHeight);
			const int fittedWidth = static_cast<int>(imageWidth * fit);
			const int fittedHeight = static_cast<int>(imageHeight * fit);
			const SDL_Rect destination{
				artwork.x + (artwork.w - fittedWidth) / 2,
				artwork.y + (artwork.h - fittedHeight) / 2,
				fittedWidth,
				fittedHeight
			};
			SDL_RenderCopy(renderer, texture, nullptr, &destination);
		}

		const SDL_Color ink{ 56, 73, 57, 255 };
		drawPixelText(renderer, exhibits[selected].title,
			(WINDOW_W - pixelTextWidth(exhibits[selected].title, 3)) / 2, 650, 3, ink);
		const std::string position = "0" + std::to_string(selected + 1) + "/05";
		drawPixelText(renderer, position,
			(WINDOW_W - pixelTextWidth(position, 2)) / 2, 690, 2, { 111, 120, 91, 255 });

		SDL_SetRenderDrawColor(renderer, 255, 250, 231, 255);
		SDL_RenderFillRect(renderer, &previousButton);
		SDL_RenderFillRect(renderer, &nextButton);
		SDL_SetRenderDrawColor(renderer, 194, 174, 125, 255);
		SDL_RenderDrawRect(renderer, &previousButton);
		SDL_RenderDrawRect(renderer, &nextButton);
		drawArrow(renderer, previousButton, false);
		drawArrow(renderer, nextButton, true);
		const std::string navigation = "LEFT / RIGHT TO BROWSE";
		const std::string returnHint = "F4 TO RETURN";
		drawPixelText(renderer, navigation,
			(WINDOW_W - pixelTextWidth(navigation, 2)) / 2, 775, 2, { 78, 100, 74, 255 });
		drawPixelText(renderer, returnHint,
			(WINDOW_W - pixelTextWidth(returnHint, 2)) / 2, 815, 2, { 78, 100, 74, 255 });
	}

private:
	void select(const int direction) {
		const int count = static_cast<int>(exhibits.size());
		selected = static_cast<std::size_t>((static_cast<int>(selected) + direction + count) % count);
	}
};

std::unique_ptr<GameScene> createGraphicsRoomScene(GraphicsRoomSceneContext context) {
	return std::make_unique<GraphicsRoomScene>(std::move(context));
}
