#include "game_scene_creators.h"
#include "game_scene_utils.h"
#include <utility>

struct GameOverSceneContext {
	SDL_Renderer* renderer;
	GameAssets& assets;
	std::function<void(const std::string&, int)> playMusic;
	int& temporaryMoney;
	std::function<void()> initialize;
	bool& hardMode;
	std::function<void(Escena)> changeScene;
	Escena& currentScene;
	bool& isClicking;
	Button& soundButton;
	SDL_Rect* mouse;
	std::function<void()> toggleMute;
	Camera& camera;
	Player& player;
};

class GameOverScene final : public GameScene {
	GameOverSceneContext context;
	public:
		explicit GameOverScene(GameOverSceneContext context) : context(std::move(context)) {}
		Escena id() const override { return GAMEOVER; }

		void enter(Escena) override {
			context.playMusic("Game Over", 1);
			context.temporaryMoney = 0;
		}

		void exit(Escena) override {
			context.hardMode = false;
			context.initialize();
		}

		void handleInput(const SDL_Event& event) override {
			if (event.type == SDL_KEYDOWN && !event.key.repeat && isConfirmKey(event.key.keysym.sym)) {
				context.changeScene(MENU);
			}
		}

		void handleClick() override {
			if (!beginClick(context.isClicking, context.soundButton, context.mouse, context.toggleMute)) return;
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
			SDL_SetRenderDrawColor(context.renderer, 0, 0, 0, 200);
			fillRect(context.renderer, { 0, 0, WINDOW_W, WINDOW_H });
			SDL_QueryTexture(context.assets.images.get("gameoverT"), nullptr, nullptr, &width, &height);
			renderTexture(context.renderer, context.assets.images.get("gameoverT"), {
				WINDOW_W - 100 - width / 3, 100, width * 1 / 3, height * 4 / 10
			});
			renderTexture(context.renderer, context.assets.images.get("linksad"), {
				(WINDOW_W / 2) - 200, (WINDOW_H / 2), 350, 350
			});
		}
	};

std::unique_ptr<GameScene> createGameOverScene(const GameSceneFactoryContext& bindings) {
	return std::make_unique<GameOverScene>(GameOverSceneContext{
		bindings.renderer,
		bindings.assets,
		bindings.playMusic,
		bindings.temporaryMoney,
		bindings.initialize,
		bindings.hardMode,
		bindings.changeScene,
		bindings.currentScene,
		bindings.isClicking,
		bindings.soundButton,
		bindings.mouse,
		bindings.toggleMute,
		bindings.camera,
		bindings.player
	});
}
