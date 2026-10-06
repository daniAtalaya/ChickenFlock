#include "shop_scene_factory.h"
#include "game/game_scene_utils.h"
#include <array>
#include <utility>

namespace {
	constexpr std::array<const char*, 9> shopDialogue{{
		"Best cocks from north to east? Juicy.",
		"God, have you seen those gorgeous wings? And prices, I'm sure you're hungry too, huh.",
		"This rod does not work as it used to, if you know what I mean.",
		"You can touch my cocks if you wanna pay for them.",
		"Not my day, mate. Fishes are leaving sea, and I'm leaving these prices almost for free, you know.",
		"Fishing was my only hope for living. Now I sell cocks. Buy them, please.",
		"You can touch my cocks if you wanna pay for them.",
		"No golems here, mate.",
		"..."
	}};
}

namespace {
	class ShopScene final : public GameScene {
		ShopSceneContext context;
		Button soundButton;
		Button backButton;
		Button buyBrownButton;
		Button exitShopButton;
		Button buyBlueButton;
		Button buyDarkButton;
		Button buyGoldenButton;
	public:
		explicit ShopScene(ShopSceneContext context) : context(std::move(context)) {
			assignRect(exitShopButton.dstRect, { 335, 650, 305, 65 });
			assignRect(buyBlueButton.dstRect, { 535, 320, 130, 40 });
			assignRect(buyGoldenButton.dstRect, { 535, 540, 130, 40 });
			assignRect(buyDarkButton.dstRect, { 320, 540, 130, 40 });
			assignRect(buyBrownButton.dstRect, { 320, 320, 130, 40 });
			soundButton.img = this->context.assets.images.get("soundOn");
			assignRect(soundButton.dstRect, { 10, 20, 100, 100 });
			backButton.img = this->context.assets.images.get("back");
			assignRect(backButton.dstRect, { 10, 135, 100, 100 });
		}

		Escena id() const override {
			return TIENDA;
		}

		void enter(Escena) override {
			context.playMusic("Tienda", -1);
			context.player.dstRect->y = WINDOW_H - 120;
			buyBrownButton.img = context.progress.brownChickenUnlocked ? context.assets.images.get("soldOut") : nullptr;
			buyBlueButton.img = context.progress.blueChickenUnlocked ? context.assets.images.get("soldOut") : nullptr;
			buyDarkButton.img = context.progress.darkChickenUnlocked ? context.assets.images.get("soldOut") : nullptr;
			buyGoldenButton.img = context.progress.goldenChickenUnlocked ? context.assets.images.get("soldOut") : nullptr;
			if (++loreShown > 9) {
				loreShown = 1;
			}
		}

		void handleInput(const SDL_Event& event) override {
			if (event.type == SDL_KEYDOWN && !event.key.repeat && event.key.keysym.sym == SDLK_q) {
				context.changeScene(MENU);
			}
		}

		void handleClick(const SDL_Point& position) override {
			if (soundButton.isClicked(position)) context.toggleMute();
			if (backButton.isClicked(position)) {
				context.changeScene(MENU);
				return;
			}
			if (exitShopButton.isClicked(position)) {
				context.changeScene(MENU);
				return;
			}
			purchase(buyBrownButton, context.progress.brownChickenUnlocked, 30, position);
			purchase(buyBlueButton, context.progress.blueChickenUnlocked, 70, position);
			purchase(buyDarkButton, context.progress.darkChickenUnlocked, 100, position);
			purchase(buyGoldenButton, context.progress.goldenChickenUnlocked, 150, position);
		}

		void render(SDL_Renderer* renderer, const bool showHitboxes) override {
			int width, height;
			context.camera.sY = 0;
			context.camera.draw(renderer, showHitboxes);
			context.player.draw(renderer, showHitboxes);
			SDL_QueryTexture(context.assets.images.get("popupTienda"), nullptr, nullptr, &width, &height);
			renderTexture(renderer, context.assets.images.get("popupTienda"), {
				              WINDOW_W / 4, 20, width - 100, height - 100
			              });
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 32);
			fillRect(renderer, { 0, 0, WINDOW_W, WINDOW_H });
			if (loreShown == 0) loreShown = 1;
			constexpr SDL_Rect loreCard{ WINDOW_W - 320, 600, 300, 264 };
			renderTexture(renderer, context.assets.images.get("tiendalore1"), loreCard);
			SDL_SetRenderDrawColor(renderer, 255, 239, 190, 255);
			fillRect(renderer, { loreCard.x + 152, loreCard.y + 36, 142, 113 });
			fillRect(renderer, { loreCard.x + 28, loreCard.y + 148, 266, 109 });
			drawWrappedPixelText(
				renderer,
				shopDialogue[static_cast<std::size_t>(loreShown - 1)],
				{ loreCard.x + 35, loreCard.y + 154, 252, 99 },
				2, { 35, 27, 20, 255 }
			);
			exitShopButton.draw(renderer, showHitboxes);
			buyBlueButton.draw(renderer, showHitboxes);
			buyGoldenButton.draw(renderer, showHitboxes);
			buyDarkButton.draw(renderer, showHitboxes);
			buyBrownButton.draw(renderer, showHitboxes);
			backButton.draw(renderer, showHitboxes);
			soundButton.img = context.assets.images.get(context.muted ? "soundOff" : "soundOn");
			soundButton.draw(renderer, showHitboxes);
			renderCurrencyPanel(renderer, context.assets, context.progress.rupees);
		}

	private:
		void purchase(Button& button, bool& unlocked, const int price, const SDL_Point& position) const {
			if (!button.isClicked(position) || unlocked || context.progress.rupees < price) {
				return;
			}
			context.progress.rupees -= price;
			unlocked = true;
			button.img = context.assets.images.get("soldOut");
		}

		int loreShown = 0;
	};
}

std::unique_ptr<GameScene> createShopScene(ShopSceneContext context) {
	return std::make_unique<ShopScene>(std::move(context));
}