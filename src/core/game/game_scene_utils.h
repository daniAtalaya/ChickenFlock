#pragma once

#include "resources/game_assets.h"
#include "player/player.h"
#include "ui/button.h"
#include <algorithm>
#include <array>
#include <cctype>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

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

inline std::array<unsigned char, 7> pixelGlyph(const char character) {
	switch (static_cast<char>(std::toupper(static_cast<unsigned char>(character)))) {
	case 'A': return { 14, 17, 17, 31, 17, 17, 17 };
	case 'B': return { 30, 17, 17, 30, 17, 17, 30 };
	case 'C': return { 14, 17, 16, 16, 16, 17, 14 };
	case 'D': return { 30, 17, 17, 17, 17, 17, 30 };
	case 'E': return { 31, 16, 16, 30, 16, 16, 31 };
	case 'F': return { 31, 16, 16, 30, 16, 16, 16 };
	case 'G': return { 14, 17, 16, 23, 17, 17, 15 };
	case 'H': return { 17, 17, 17, 31, 17, 17, 17 };
	case 'I': return { 14, 4, 4, 4, 4, 4, 14 };
	case 'J': return { 7, 2, 2, 2, 2, 18, 12 };
	case 'K': return { 17, 18, 20, 24, 20, 18, 17 };
	case 'L': return { 16, 16, 16, 16, 16, 16, 31 };
	case 'M': return { 17, 27, 21, 21, 17, 17, 17 };
	case 'N': return { 17, 25, 21, 19, 17, 17, 17 };
	case 'O': return { 14, 17, 17, 17, 17, 17, 14 };
	case 'P': return { 30, 17, 17, 30, 16, 16, 16 };
	case 'Q': return { 14, 17, 17, 17, 21, 18, 13 };
	case 'R': return { 30, 17, 17, 30, 20, 18, 17 };
	case 'S': return { 15, 16, 16, 14, 1, 1, 30 };
	case 'T': return { 31, 4, 4, 4, 4, 4, 4 };
	case 'U': return { 17, 17, 17, 17, 17, 17, 14 };
	case 'V': return { 17, 17, 17, 17, 17, 10, 4 };
	case 'W': return { 17, 17, 17, 21, 21, 21, 10 };
	case 'X': return { 17, 17, 10, 4, 10, 17, 17 };
	case 'Y': return { 17, 17, 10, 4, 4, 4, 4 };
	case 'Z': return { 31, 1, 2, 4, 8, 16, 31 };
	case '0': return { 14, 17, 19, 21, 25, 17, 14 };
	case '1': return { 4, 12, 4, 4, 4, 4, 14 };
	case '2': return { 14, 17, 1, 2, 4, 8, 31 };
	case '3': return { 30, 1, 1, 14, 1, 1, 30 };
	case '4': return { 2, 6, 10, 18, 31, 2, 2 };
	case '5': return { 31, 16, 16, 30, 1, 1, 30 };
	case '6': return { 14, 16, 16, 30, 17, 17, 14 };
	case '7': return { 31, 1, 2, 4, 8, 8, 8 };
	case '8': return { 14, 17, 17, 14, 17, 17, 14 };
	case '9': return { 14, 17, 17, 15, 1, 1, 14 };
	case '/': return { 1, 1, 2, 4, 8, 16, 16 };
	case '.': return { 0, 0, 0, 0, 0, 12, 12 };
	case ',': return { 0, 0, 0, 0, 12, 12, 8 };
	case '\'': return { 4, 4, 8, 0, 0, 0, 0 };
	case '"': return { 10, 10, 0, 0, 0, 0, 0 };
	case '?': return { 14, 17, 1, 2, 4, 0, 4 };
	case '!': return { 4, 4, 4, 4, 4, 0, 4 };
	case ':': return { 0, 12, 12, 0, 12, 12, 0 };
	case '-': return { 0, 0, 0, 31, 0, 0, 0 };
	case '_': return { 0, 0, 0, 0, 0, 0, 31 };
	case '(': return { 2, 4, 8, 8, 8, 4, 2 };
	case ')': return { 8, 4, 2, 2, 2, 4, 8 };
	case '+': return { 0, 4, 4, 31, 4, 4, 0 };
	case '=': return { 0, 31, 0, 31, 0, 0, 0 };
	case '&': return { 12, 18, 20, 8, 21, 18, 13 };
	case '%': return { 17, 2, 4, 8, 16, 17, 0 };
	case '#': return { 10, 31, 10, 10, 31, 10, 0 };
	case '*': return { 0, 21, 14, 31, 14, 21, 0 };
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
		for (int row = 0; row < 7; ++row) {
			for (int column = 0; column < 5; ++column) {
				if ((rows[row] & (1U << (4 - column))) == 0) continue;
				const SDL_Rect pixel{ x + column * scale, y + row * scale, scale, scale };
				SDL_RenderFillRect(renderer, &pixel);
			}
		}
		x += 6 * scale;
	}
}

inline void drawWrappedPixelText(SDL_Renderer* renderer, const std::string& text,
	const SDL_Rect& bounds, const int scale, const SDL_Color color) {
	const int maxCharacters = std::max(1, bounds.w / (6 * scale));
	std::istringstream words(text);
	std::vector<std::string> lines;
	std::string word;
	std::string line;
	while (words >> word) {
		if (word.size() > static_cast<std::size_t>(maxCharacters)) {
			if (!line.empty()) {
				lines.push_back(std::move(line));
				line.clear();
			}
			for (std::size_t offset = 0; offset < word.size(); offset += static_cast<std::size_t>(maxCharacters)) {
				lines.push_back(word.substr(offset, static_cast<std::size_t>(maxCharacters)));
			}
			continue;
		}
		const int addedWidth = static_cast<int>(line.size() + word.size() + (line.empty() ? 0 : 1));
		if (!line.empty() && addedWidth > maxCharacters) {
			lines.push_back(std::move(line));
			line.clear();
		}
		if (!line.empty()) line.push_back(' ');
		line.append(word);
	}
	if (!line.empty()) lines.push_back(std::move(line));
	const int lineHeight = 9 * scale;
	const std::size_t visibleLines = static_cast<std::size_t>(std::max(0, bounds.h / lineHeight));
	const std::size_t lineCount = std::min(lines.size(), visibleLines);
	for (std::size_t index = 0; index < lineCount; ++index) {
		drawPixelText(renderer, lines[index], bounds.x, bounds.y + static_cast<int>(index) * lineHeight,
			scale, color);
	}
}

inline void renderCurrencyPanel(SDL_Renderer* renderer, GameAssets& assets,
	const int banked, const int carried = 0) {
	const bool showCarried = carried > 0;
	const SDL_Rect panel{ 120, 20, 190, showCarried ? 100 : 56 };
	SDL_SetRenderDrawColor(renderer, 255, 239, 190, 245);
	SDL_RenderFillRect(renderer, &panel);
	SDL_SetRenderDrawColor(renderer, 127, 82, 38, 255);
	SDL_RenderDrawRect(renderer, &panel);

	const SDL_Rect bankedIcon{ panel.x + 12, panel.y + 10, 32, 32 };
	SDL_RenderCopy(renderer, assets.images.get("rupia1"), nullptr, &bankedIcon);
	const std::string bankedText = std::to_string(std::max(0, banked));
	drawPixelText(renderer, bankedText, panel.x + 54, panel.y + 17, 2, { 44, 67, 53, 255 });
	if (showCarried) {
		SDL_SetRenderDrawColor(renderer, 127, 82, 38, 100);
		SDL_RenderDrawLine(renderer, panel.x + 10, panel.y + 49, panel.x + panel.w - 10, panel.y + 49);
		const SDL_Rect carriedIcon{ panel.x + 12, panel.y + 58, 32, 32 };
		SDL_RenderCopy(renderer, assets.images.get("rupia2"), nullptr, &carriedIcon);
		const std::string carriedText = std::to_string(carried);
		drawPixelText(renderer, carriedText, panel.x + 54, panel.y + 65, 2, { 44, 67, 53, 255 });
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
