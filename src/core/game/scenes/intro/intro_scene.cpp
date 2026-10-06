#include "intro_scene_factory.h"
#include "game/game_scene_utils.h"
#include <utility>

class IntroScene final : public GameScene {
	IntroSceneContext context;
	Button soundButton;
	public:
		explicit IntroScene(IntroSceneContext context) : context(std::move(context)) {
			assignRect(soundButton.dstRect, { 10, 20, 100, 100 });
		}
		Escena id() const override { return INICI; }

		void handleInput(const SDL_Event& event) override {
			if (event.type == SDL_EVENT_KEY_DOWN && !event.key.repeat && isConfirmKey(event.key.key)) {
				context.changeScene(MENU);
			}
		}

		void handleClick(const SDL_Point& position) override {
			if (soundButton.isClicked(position)) context.toggleMute();
		}

		void render(SDL_Renderer* renderer, bool) override {
			int width, height;
			soundButton.img = context.assets.images.get(context.muted ? "soundOff" : "soundOn");
			soundButton.draw(renderer);
			getTextureSize(context.assets.images.get("studio"), &width, &height);
			renderTexture(renderer, context.assets.images.get("studio"), { (WINDOW_W / 2) - 220, 150, 440, 440 });
			getTextureSize(context.assets.images.get("enter"), &width, &height);
			renderTexture(renderer, context.assets.images.get("enter"), {
				WINDOW_W - 55 - width / 2, 550, width / 2, height * 7 / 10
			});
		}
	};

std::unique_ptr<GameScene> createIntroScene(IntroSceneContext context) {
	return std::make_unique<IntroScene>(std::move(context));
}
