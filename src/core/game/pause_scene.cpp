#include "game_scene_creators.h"
#include "game_scene_utils.h"
#include <utility>

struct PauseSceneContext {
	SDL_Renderer* renderer;
	GameAssets& assets;
	std::function<void()> haltChannels;
	std::function<void(Escena)> changeScene;
	Escena& currentScene;
	bool& isClicking;
	Button& soundButton;
	SDL_Rect* mouse;
	std::function<void()> toggleMute;
	Button& backButton;
	Button& shopButton;
	Button& hardcoreButton;
	Camera& camera;
	Player& player;
	Button& playButton;
	GameWorld& world;
};

class PauseScene final : public GameScene {
	PauseSceneContext context;
	public:
		explicit PauseScene(PauseSceneContext context) : context(std::move(context)) {}
		Escena id() const override { return PAUSA; }

		void enter(Escena) override {
			context.haltChannels();
		}

		void exit(Escena nextScene) override {
			if (nextScene != JOC) {
				context.world.markForRemoval();
			}
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
			context.isClicking = false;
		}

		void render() override {
			int width, height;
			context.camera.draw();
			context.player.draw();
			SDL_SetRenderDrawColor(context.renderer, 0, 0, 0, 128);
			fillRect(context.renderer, { 0, 0, WINDOW_W, WINDOW_H });
			context.soundButton.draw();
			context.playButton.draw();
			context.backButton.draw();
			renderHearts(context.player);
			if (SDL_GetTicks() / 16 % 40 == 0) {
				showPauseText = !showPauseText;
			}
			if (showPauseText) {
				SDL_QueryTexture(context.assets.images.get("pausaT"), nullptr, nullptr, &width, &height);
				renderTexture(context.renderer, context.assets.images.get("pausaT"), {
					(WINDOW_W / 2) - 320, WINDOW_H / 2 - height * 2 / 10,
					width * 1 / 3, height * 4 / 10
				});
			}
		}

	private:
		bool showPauseText = true;
	};

std::unique_ptr<GameScene> createPauseScene(const GameSceneFactoryContext& bindings) {
	return std::make_unique<PauseScene>(PauseSceneContext{
		bindings.renderer,
		bindings.assets,
		bindings.haltChannels,
		bindings.changeScene,
		bindings.currentScene,
		bindings.isClicking,
		bindings.soundButton,
		bindings.mouse,
		bindings.toggleMute,
		bindings.backButton,
		bindings.shopButton,
		bindings.hardcoreButton,
		bindings.camera,
		bindings.player,
		bindings.playButton,
		bindings.world
	});
}
