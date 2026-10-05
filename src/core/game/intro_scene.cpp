#include "game_scene_creators.h"
#include "game_scene_utils.h"
#include <utility>

struct IntroSceneContext {
	SDL_Renderer* renderer;
	GameAssets& assets;
	std::function<void(Escena)> changeScene;
	bool& isClicking;
	Button& soundButton;
	SDL_Rect* mouse;
	std::function<void()> toggleMute;
};

class IntroScene final : public GameScene {
	IntroSceneContext context;
	public:
		explicit IntroScene(IntroSceneContext context) : context(std::move(context)) {}
		Escena id() const override { return INICI; }

		void handleInput(const SDL_Event& event) override {
			if (event.type == SDL_KEYDOWN && !event.key.repeat && isConfirmKey(event.key.keysym.sym)) {
				context.changeScene(MENU);
			}
		}

		void handleClick() override {
			if (beginClick(context.isClicking, context.soundButton, context.mouse, context.toggleMute)) {
				context.isClicking = false;
			}
		}

		void render() override {
			int width, height;
			SDL_QueryTexture(context.assets.images.get("studio"), nullptr, nullptr, &width, &height);
			renderTexture(context.renderer, context.assets.images.get("studio"), { (WINDOW_W / 2) - 220, 150, 440, 440 });
			SDL_QueryTexture(context.assets.images.get("enter"), nullptr, nullptr, &width, &height);
			renderTexture(context.renderer, context.assets.images.get("enter"), {
				WINDOW_W - 55 - width / 2, 550, width / 2, height * 7 / 10
			});
		}
	};

std::unique_ptr<GameScene> createIntroScene(const GameSceneFactoryContext& bindings) {
	return std::make_unique<IntroScene>(IntroSceneContext{
		bindings.renderer,
		bindings.assets,
		bindings.changeScene,
		bindings.isClicking,
		bindings.soundButton,
		bindings.mouse,
		bindings.toggleMute
	});
}
