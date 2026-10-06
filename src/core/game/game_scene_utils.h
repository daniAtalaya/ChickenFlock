#pragma once

#include "resources/game_assets.h"
#include "player/player.h"
#include "ui/button.h"
#include <array>
#include <string>

inline void renderTexture(SDL_Renderer* renderer, SDL_Texture* texture, const SDL_Rect& destination) {
	SDL_RenderCopy(renderer, texture, nullptr, &destination);
}

inline void fillRect(SDL_Renderer* renderer, const SDL_Rect& rectangle) {
	SDL_RenderFillRect(renderer, &rectangle);
}

inline void assignRect(SDL_Rect*& target, const SDL_Rect& value) {
	if (target == nullptr) {
		target = new SDL_Rect(value);
	} else {
		*target = value;
	}
}

inline void renderMoney(SDL_Renderer* renderer, GameAssets& assets, int amount, const int y = 88) {
	const std::string digits = std::to_string(amount);
	for (int i = static_cast<int>(digits.length()) - 1; i >= 0; --i) {
		std::string image = "n";
		image.append(1, digits[i]);
		const SDL_Rect destination{
			WINDOW_W - 80 - (static_cast<int>(digits.length()) - i) * 30,
			y,
			35,
			40
		};
		SDL_RenderCopy(renderer, assets.images.get(image), nullptr, &destination);
	}
}

inline void renderCurrencyPanel(SDL_Renderer* renderer, GameAssets& assets,
	const int banked, const int carried = 0) {
	const bool showCarried = carried > 0;
	const SDL_Rect panel{ WINDOW_W - 250, 20, 230, showCarried ? 104 : 58 };
	SDL_SetRenderDrawColor(renderer, 255, 239, 190, 245);
	SDL_RenderFillRect(renderer, &panel);
	SDL_SetRenderDrawColor(renderer, 127, 82, 38, 255);
	SDL_RenderDrawRect(renderer, &panel);

	const SDL_Rect bankedIcon{ WINDOW_W - 55, 32, 30, 30 };
	SDL_RenderCopy(renderer, assets.images.get("rupia1"), nullptr, &bankedIcon);
	renderMoney(renderer, assets, banked, 28);
	if (showCarried) {
		SDL_SetRenderDrawColor(renderer, 127, 82, 38, 100);
		SDL_RenderDrawLine(renderer, panel.x + 10, panel.y + 48, panel.x + panel.w - 10, panel.y + 48);
		const SDL_Rect carriedIcon{ WINDOW_W - 55, 80, 30, 30 };
		SDL_RenderCopy(renderer, assets.images.get("rupia2"), nullptr, &carriedIcon);
		renderMoney(renderer, assets, carried, 76);
	}
}

inline std::array<unsigned char, 5> pixelGlyph(const char character) {
	switch (character) {
	case 'A': return { 14, 17, 31, 17, 17 };
	case 'B': return { 30, 17, 30, 17, 30 };
	case 'C': return { 14, 17, 16, 17, 14 };
	case 'D': return { 30, 17, 17, 17, 30 };
	case 'E': return { 31, 16, 30, 16, 31 };
	case 'F': return { 31, 16, 30, 16, 16 };
	case 'G': return { 14, 17, 23, 17, 15 };
	case 'H': return { 17, 17, 31, 17, 17 };
	case 'I': return { 31, 4, 4, 4, 31 };
	case 'J': return { 7, 2, 2, 18, 12 };
	case 'K': return { 17, 18, 28, 18, 17 };
	case 'L': return { 16, 16, 16, 16, 31 };
	case 'M': return { 17, 27, 21, 17, 17 };
	case 'N': return { 17, 25, 21, 19, 17 };
	case 'O': return { 14, 17, 17, 17, 14 };
	case 'P': return { 30, 17, 30, 16, 16 };
	case 'Q': return { 14, 17, 17, 21, 10 };
	case 'R': return { 30, 17, 30, 18, 17 };
	case 'S': return { 15, 16, 14, 1, 30 };
	case 'T': return { 31, 4, 4, 4, 4 };
	case 'U': return { 17, 17, 17, 17, 14 };
	case 'V': return { 17, 17, 17, 10, 4 };
	case 'W': return { 17, 17, 21, 27, 17 };
	case 'X': return { 17, 10, 4, 10, 17 };
	case 'Y': return { 17, 10, 4, 4, 4 };
	case '0': return { 14, 19, 21, 25, 14 };
	case '1': return { 4, 12, 4, 4, 14 };
	case '2': return { 14, 17, 2, 4, 31 };
	case '3': return { 30, 1, 6, 1, 30 };
	case '4': return { 2, 6, 10, 31, 2 };
	case '5': return { 31, 16, 30, 1, 30 };
	case '/': return { 1, 2, 4, 8, 16 };
	default: return {};
	}
}

inline int pixelTextWidth(const std::string& text, const int scale) {
	return text.empty() ? 0 : static_cast<int>(text.size()) * 6 * scale - scale;
}

inline void drawPixelText(SDL_Renderer* renderer, const std::string& text, int x, const int y,
	const int scale, const SDL_Color color) {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	for (const char character : text) {
		const auto rows = pixelGlyph(character);
		for (int row = 0; row < 5; ++row) {
			for (int column = 0; column < 5; ++column) {
				if ((rows[row] & (1U << (4 - column))) == 0) continue;
				const SDL_Rect pixel{ x + column * scale, y + row * scale, scale, scale };
				SDL_RenderFillRect(renderer, &pixel);
			}
		}
		x += 6 * scale;
	}
}

inline bool isConfirmKey(SDL_Keycode key) {
	return key == SDLK_RETURN || key == SDLK_SPACE;
}

inline void renderHearts(SDL_Renderer* renderer, Player& player, const bool showHitboxes = false) {
	for (int i = 0; i < 3; ++i) {
		player.corazones[i].draw(renderer, showHitboxes);
	}
}
