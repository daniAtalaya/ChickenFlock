#include "victory_scene_factory.h"
#include "game/game_scene_utils.h"
#include <utility>

class VictoryScene final : public GameScene {
	VictorySceneContext context;
	Button soundButton;
	public:
		explicit VictoryScene(VictorySceneContext context) : context(std::move(context)) {
			assignRect(soundButton.dstRect, { 10, 20, 100, 100 });
		}
		Escena id() const override { return GUANYAT; }

		void enter(Escena) override {
			context.haltChannels();
			context.playMusic("Victoria", 1);
		}

		void exit(Escena) override {
			context.initialize();
		}

		void handleInput(const SDL_Event& event) override {
			if (event.type == SDL_KEYDOWN && !event.key.repeat && isConfirmKey(event.key.keysym.sym)) {
				context.changeScene(CREDITS);
			}
		}

		void handleClick(const SDL_Point& position) override {
			if (soundButton.isClicked(position)) {
				context.toggleMute();
			} else {
				context.changeScene(CREDITS);
			}
		}

		void render(SDL_Renderer* renderer, const bool showHitboxes) override {
			int width, height;
			context.camera.sY = 0;
			context.camera.draw(renderer, showHitboxes);
			context.player.draw(renderer, showHitboxes);
			soundButton.img = context.assets.images.get(context.muted ? "soundOff" : "soundOn");
			soundButton.draw(renderer, showHitboxes);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
			fillRect(renderer, { 0, 0, WINDOW_W, WINDOW_H });
			SDL_QueryTexture(context.assets.images.get("winner"), nullptr, nullptr, &width, &height);
			renderTexture(renderer, context.assets.images.get("winner"), {
				WINDOW_W / 2 - width / 2, WINDOW_H / 2 - height / 2, width, height
			});
			renderCurrencyPanel(renderer, context.assets, context.progress.rupees);
		}
	};

std::unique_ptr<GameScene> createVictoryScene(VictorySceneContext context) {
	return std::make_unique<VictoryScene>(std::move(context));
}
