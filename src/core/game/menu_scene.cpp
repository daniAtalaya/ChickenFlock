#include "game_scene_creators.h"
#include "game_scene_utils.h"
#include <utility>

struct MenuSceneContext {
	SDL_Renderer* renderer;
	GameAssets& assets;
	Escena& currentScene;
	std::function<void()> haltChannels;
	std::function<void(const std::string&, int)> playMusic;
	int& temporaryMoney;
	std::function<void()> initialize;
	std::function<void(Escena)> changeScene;
	bool& isClicking;
	Button& soundButton;
	SDL_Rect* mouse;
	std::function<void()> toggleMute;
	Button& backButton;
	Button& shopButton;
	Button& hardcoreButton;
	bool& hardMode;
	Camera& camera;
	Player& player;
	Perro& pet;
	bool& paused;
};

class MenuScene final : public GameScene {
	MenuSceneContext context;
	Button creditsButton;
	public:
		explicit MenuScene(MenuSceneContext context) : context(std::move(context)) {
			creditsButton.img = this->context.assets.images.get("creditosBoton");
			assignRect(creditsButton.dstRect, { WINDOW_W - 410, 750, 330, 100 });
		}
		Escena id() const override { return MENU; }

		void enter(Escena) override {
			context.haltChannels();
			context.playMusic("Menu", -1);
			context.temporaryMoney = 0;
		}

		void exit(Escena) override {
			context.initialize();
		}

		void handleInput(const SDL_Event& event) override {
			if (event.type != SDL_KEYDOWN || event.key.repeat) {
				return;
			}
			const SDL_Keycode key = event.key.keysym.sym;
			if (isConfirmKey(key)) {
				context.changeScene(LORE);
			}
			if (key == SDLK_t) {
				context.changeScene(TIENDA);
			}
		}

		void handleClick() override {
			if (!beginClick(context.isClicking, context.soundButton, context.mouse, context.toggleMute)) {
				return;
			}
			if (creditsButton.isClicked(context.mouse) && context.currentScene == MENU) {
				context.changeScene(CREDITS);
			}
			if (context.backButton.isClicked(context.mouse)
				&& (context.currentScene == PAUSA || context.currentScene == TIENDA)) {
				context.changeScene(MENU);
			} else if (context.shopButton.isClicked(context.mouse) && context.currentScene == MENU) {
				context.changeScene(TIENDA);
			}
			if (context.hardcoreButton.isClicked(context.mouse) && context.currentScene == MENU && !context.hardMode) {
				context.hardMode = !context.hardMode;
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
			context.isClicking = false;
		}

		void render() override {
			int width, height;
			context.camera.sY = 0;
			context.camera.draw();
			context.player.draw();
			SDL_SetRenderDrawColor(context.renderer, 0, 0, 0, 128);
			fillRect(context.renderer, { 0, 0, WINDOW_W, WINDOW_H });
			context.soundButton.draw();
			context.shopButton.draw();
			SDL_QueryTexture(context.assets.images.get("start"), nullptr, nullptr, &width, &height);
			renderTexture(context.renderer, context.assets.images.get("start"), {
				WINDOW_W - 100 - width / 3, (WINDOW_H / 2) - (height * 4 / 10) / 2,
				width * 1 / 3, height * 4 / 10
			});
			SDL_QueryTexture(context.assets.images.get("tituloCockFlock"), nullptr, nullptr, &width, &height);
			renderTexture(context.renderer, context.assets.images.get("tituloCockFlock"), {
				(WINDOW_W / 2) - 160, 50, width / 2, height / 2
			});
			creditsButton.draw();
			if (!context.hardMode) {
				context.hardcoreButton.draw();
			}
			if (context.player.gallinasDesbloqueadas == 5) {
				context.pet.draw();
				if ((SDL_GetTicks() / 16) % 20 == 0) context.pet.animateX();
				if ((SDL_GetTicks() / 16) % (100 * context.pet.spritesheet.maxC) == 0 && !context.paused) {
					context.pet.animateY();
				}
			}
		}
	};

std::unique_ptr<GameScene> createMenuScene(const GameSceneFactoryContext& bindings) {
	return std::make_unique<MenuScene>(MenuSceneContext{
		bindings.renderer,
		bindings.assets,
		bindings.currentScene,
		bindings.haltChannels,
		bindings.playMusic,
		bindings.temporaryMoney,
		bindings.initialize,
		bindings.changeScene,
		bindings.isClicking,
		bindings.soundButton,
		bindings.mouse,
		bindings.toggleMute,
		bindings.backButton,
		bindings.shopButton,
		bindings.hardcoreButton,
		bindings.hardMode,
		bindings.camera,
		bindings.player,
		bindings.pet,
		bindings.paused
	});
}
