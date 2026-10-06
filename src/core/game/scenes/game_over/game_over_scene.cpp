#include "game_over_scene_factory.h"
#include "game/game_scene_utils.h"
#include <utility>

class GameOverScene final : public GameScene {
	GameOverSceneContext context;
	Button soundButton;
	public:
		explicit GameOverScene(GameOverSceneContext context) : context(std::move(context)) {
			assignRect(soundButton.dstRect, { 10, 20, 100, 100 });
		}
		Escena id() const override { return GAMEOVER; }

		void enter(Escena) override {
			context.playMusic("Game Over", 1);
		}

		void exit(Escena) override {
			context.initialize();
		}

		void handleInput(const SDL_Event& event) override {
			if (event.type == SDL_KEYDOWN && !event.key.repeat && isConfirmKey(event.key.keysym.sym)) {
				context.changeScene(MENU);
			}
		}

		void handleClick(const SDL_Point& position) override {
			if (soundButton.isClicked(position)) {
				context.toggleMute();
			} else {
				context.changeScene(MENU);
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
			SDL_QueryTexture(context.assets.images.get("gameoverT"), nullptr, nullptr, &width, &height);
			renderTexture(renderer, context.assets.images.get("gameoverT"), {
				WINDOW_W - 100 - width / 3, 100, width * 1 / 3, height * 4 / 10
			});
			renderTexture(renderer, context.assets.images.get("linksad"), {
				(WINDOW_W / 2) - 200, (WINDOW_H / 2), 350, 350
			});
			renderCurrencyPanel(renderer, context.assets, context.progress.rupees);
		}
	};

std::unique_ptr<GameScene> createGameOverScene(GameOverSceneContext context) {
	return std::make_unique<GameOverScene>(std::move(context));
}
