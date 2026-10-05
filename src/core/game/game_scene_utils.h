#pragma once

#include "resources/game_assets.h"
#include "player/player.h"
#include "ui/button.h"
#include <functional>
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

inline void renderMoney(SDL_Renderer* renderer, GameAssets& assets, int amount) {
	const std::string digits = std::to_string(amount);
	for (int i = static_cast<int>(digits.length()) - 1; i >= 0; --i) {
		std::string image = "n";
		image.append(1, digits[i]);
		const SDL_Rect destination{
			WINDOW_W - 80 - (static_cast<int>(digits.length()) - i) * 30,
			88,
			35,
			40
		};
		SDL_RenderCopy(renderer, assets.images.get(image), nullptr, &destination);
	}
}

inline bool isConfirmKey(SDL_Keycode key) {
	return key == SDLK_RETURN || key == SDLK_SPACE;
}

inline bool beginClick(bool& isClicking, Button& soundButton, SDL_Rect* mouse,
	const std::function<void()>& toggleMute) {
	if (!isClicking) {
		return false;
	}
	if (soundButton.isClicked(mouse)) {
		toggleMute();
	}
	return true;
}

inline void renderHearts(Player& player) {
	for (int i = 0; i < 3; ++i) {
		player.corazones[i].draw();
	}
}
