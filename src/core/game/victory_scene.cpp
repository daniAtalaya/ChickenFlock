#include "game_scene_creators.h"
#include "game_scene_utils.h"
#include <utility>

struct VictorySceneContext {
	SDL_Renderer* renderer;
	GameAssets& assets;
	std::function<void()> haltChannels;
	std::function<void(const std::string&, int)> playMusic;
	bool& hardMode;
	std::function<void()> initialize;
	std::function<void(Escena)> changeScene;
	Escena& currentScene;
	bool& isClicking;
	Button& soundButton;
	SDL_Rect* mouse;
	std::function<void()> toggleMute;
	Camera& camera;
	Player& player;
};

class VictoryScene final : public GameScene {
	VictorySceneContext context;
	public:
		explicit VictoryScene(VictorySceneContext context) : context(std::move(context)) {}
		Escena id() const override { return GUANYAT; }

		void enter(Escena) override {
			context.haltChannels();
			context.playMusic("Victoria", 1);
		}

		void exit(Escena) override {
			context.hardMode = false;
			context.initialize();
		}

		void handleInput(const SDL_Event& event) override {
			if (event.type == SDL_KEYDOWN && !event.key.repeat && isConfirmKey(event.key.keysym.sym)) {
				context.changeScene(CREDITS);
			}
		}

		void handleClick() override {
			if (!beginClick(context.isClicking, context.soundButton, context.mouse, context.toggleMute)) return;
			if (context.currentScene == GUANYAT) {
				context.changeScene(CREDITS);
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
			SDL_QueryTexture(context.assets.images.get("winner"), nullptr, nullptr, &width, &height);
			renderTexture(context.renderer, context.assets.images.get("winner"), {
				WINDOW_W / 2 - width / 2, WINDOW_H / 2 - height / 2, width, height
			});
		}
	};

std::unique_ptr<GameScene> createVictoryScene(const GameSceneFactoryContext& bindings) {
	return std::make_unique<VictoryScene>(VictorySceneContext{
		bindings.renderer,
		bindings.assets,
		bindings.haltChannels,
		bindings.playMusic,
		bindings.hardMode,
		bindings.initialize,
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
