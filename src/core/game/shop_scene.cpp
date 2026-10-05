#include "game_scene_creators.h"
#include "game_scene_utils.h"
#include <utility>

struct ShopSceneContext {
	SDL_Renderer* renderer;
	GameAssets& assets;
	Button& backButton;
	std::function<void(const std::string&, int)> playMusic;
	Player& player;
	std::function<void(Escena)> changeScene;
	Escena& currentScene;
	bool& isClicking;
	Button& soundButton;
	SDL_Rect* mouse;
	std::function<void()> toggleMute;
	Button& shopButton;
	Button& hardcoreButton;
	Camera& camera;
	std::function<void()> initialize;
};

class ShopScene final : public GameScene {
	ShopSceneContext context;
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
		}
		Escena id() const override { return TIENDA; }

		void enter(Escena) override {
			*context.backButton.dstRect = { 10, 135, 100, 100 };
			context.playMusic("Tienda", -1);
			context.player.dstRect->y = WINDOW_H - 120;
			if (++loreShown > 9) {
				loreShown = 1;
			}
		}

		void exit(Escena) override {
			context.initialize();
		}

		void handleInput(const SDL_Event& event) override {
			if (event.type == SDL_KEYDOWN && !event.key.repeat && event.key.keysym.sym == SDLK_q) {
				context.changeScene(MENU);
			}
		}

		void handleClick() override {
			if (!beginClick(context.isClicking, context.soundButton, context.mouse, context.toggleMute)) return;
			if (context.backButton.isClicked(context.mouse)
				&& (context.currentScene == PAUSA || context.currentScene == TIENDA)) {
				context.changeScene(MENU);
			} else if (context.shopButton.isClicked(context.mouse) && context.currentScene == MENU) {
				context.changeScene(TIENDA);
			}
			if (context.currentScene == MENU
				&& !context.shopButton.isClicked(context.mouse)
				&& !context.soundButton.isClicked(context.mouse)
				&& !context.hardcoreButton.isClicked(context.mouse)) {
				context.changeScene(LORE);
			}
			if (context.currentScene == GUANYAT) {
				context.changeScene(CREDITS);
			}
			if (context.currentScene == GAMEOVER) {
				context.changeScene(MENU);
			}
			if (context.currentScene == TIENDA) {
				if (buyBrownButton.isClicked(context.mouse)
					&& context.player.gallinasDesbloqueadas < 5 && !context.player.brownComprada
					&& context.player.money >= 30) {
					context.player.money -= 30;
					context.player.brownComprada = true;
					++context.player.gallinasDesbloqueadas;
					buyBrownButton.img = context.assets.images.get("soldOut");
				}
				if (exitShopButton.isClicked(context.mouse)) {
					context.changeScene(MENU);
				}
				if (buyBlueButton.isClicked(context.mouse)
					&& context.player.gallinasDesbloqueadas < 5 && !context.player.azulComprada
					&& context.player.money >= 70) {
					context.player.money -= 70;
					context.player.azulComprada = true;
					++context.player.gallinasDesbloqueadas;
					buyBlueButton.img = context.assets.images.get("soldOut");
				}
				if (buyDarkButton.isClicked(context.mouse)
					&& context.player.gallinasDesbloqueadas < 5 && !context.player.darkComprada
					&& context.player.money >= 100) {
					context.player.money -= 100;
					context.player.darkComprada = true;
					++context.player.gallinasDesbloqueadas;
					buyDarkButton.img = context.assets.images.get("soldOut");
				}
				if (buyGoldenButton.isClicked(context.mouse)
					&& context.player.gallinasDesbloqueadas < 5 && !context.player.goldenComprada
					&& context.player.money >= 150) {
					context.player.money -= 150;
					context.player.goldenComprada = true;
					++context.player.gallinasDesbloqueadas;
					buyGoldenButton.img = context.assets.images.get("soldOut");
				}
			}
			context.isClicking = false;
		}

		void render() override {
			int width, height;
			context.camera.sY = 0;
			context.camera.draw();
			context.player.draw();
			SDL_QueryTexture(context.assets.images.get("popupTienda"), nullptr, nullptr, &width, &height);
			renderTexture(context.renderer, context.assets.images.get("popupTienda"), {
				WINDOW_W / 4, 20, width - 100, height - 100
			});
			SDL_SetRenderDrawColor(context.renderer, 0, 0, 0, 32);
			fillRect(context.renderer, { 0, 0, WINDOW_W, WINDOW_H });
			if (loreShown == 0) loreShown = 1;
			const std::string lore = "tiendalore" + std::to_string(loreShown);
			SDL_QueryTexture(context.assets.images.get(lore), nullptr, nullptr, &width, &height);
			renderTexture(context.renderer, context.assets.images.get(lore), { WINDOW_W - 280, 640, width, height });
			exitShopButton.draw();
			buyBlueButton.draw();
			buyGoldenButton.draw();
			buyDarkButton.draw();
			buyBrownButton.draw();
			context.backButton.draw();
			context.soundButton.draw();
			renderTexture(context.renderer, context.assets.images.get("rupia1"), { WINDOW_W - 60, 90, 40, 40 });
			renderMoney(context.renderer, context.assets, context.player.money);
		}

	private:
		int loreShown = 0;
	};

std::unique_ptr<GameScene> createShopScene(const GameSceneFactoryContext& bindings) {
	return std::make_unique<ShopScene>(ShopSceneContext{
		bindings.renderer,
		bindings.assets,
		bindings.backButton,
		bindings.playMusic,
		bindings.player,
		bindings.changeScene,
		bindings.currentScene,
		bindings.isClicking,
		bindings.soundButton,
		bindings.mouse,
		bindings.toggleMute,
		bindings.shopButton,
		bindings.hardcoreButton,
		bindings.camera,
		bindings.initialize
	});
}
