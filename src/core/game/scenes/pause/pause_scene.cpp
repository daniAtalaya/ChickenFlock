#include "pause_scene_factory.h"
#include "game/game_scene_utils.h"
#include <utility>

class PauseScene final : public GameScene {
	PauseSceneContext context;
	Button soundButton;
	Button playButton;
	Button backButton;
	public:
		explicit PauseScene(PauseSceneContext context) : context(std::move(context)) {
			soundButton.img = this->context.assets.images.get("soundOn");
			assignRect(soundButton.dstRect, { 10, 20, 100, 100 });
			playButton.img = this->context.assets.images.get("play");
			assignRect(playButton.dstRect, { 10, 135, 100, 100 });
			backButton.img = this->context.assets.images.get("back");
			assignRect(backButton.dstRect, { 10, 250, 100, 100 });
		}
		Escena id() const override { return PAUSA; }

		void enter(Escena) override {
			context.haltChannels();
		}

		void handleInput(const SDL_Event& event) override {
			if (event.type == SDL_KEYDOWN && !event.key.repeat && event.key.keysym.sym == SDLK_q) {
				context.changeScene(MENU);
			}
		}

		void handleClick(const SDL_Point& position) override {
			if (soundButton.isClicked(position)) context.toggleMute();
			if (playButton.isClicked(position)) context.togglePause();
			if (backButton.isClicked(position)) context.changeScene(MENU);
		}

		void render(SDL_Renderer* renderer, const bool showHitboxes) override {
			int width, height;
			context.camera.draw(renderer, showHitboxes);
			context.player.draw(renderer, showHitboxes);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
			fillRect(renderer, { 0, 0, WINDOW_W, WINDOW_H });
			soundButton.img = context.assets.images.get(context.muted ? "soundOff" : "soundOn");
			soundButton.draw(renderer, showHitboxes);
			playButton.draw(renderer, showHitboxes);
			backButton.draw(renderer, showHitboxes);
			renderHearts(renderer, context.player, showHitboxes);
			if (SDL_GetTicks() / 16 % 40 == 0) {
				showPauseText = !showPauseText;
			}
			if (showPauseText) {
				SDL_QueryTexture(context.assets.images.get("pausaT"), nullptr, nullptr, &width, &height);
				renderTexture(renderer, context.assets.images.get("pausaT"), {
					(WINDOW_W / 2) - 320, WINDOW_H / 2 - height * 2 / 10,
					width * 1 / 3, height * 4 / 10
				});
			}
		}

	private:
		bool showPauseText = true;
	};

std::unique_ptr<GameScene> createPauseScene(PauseSceneContext context) {
	return std::make_unique<PauseScene>(std::move(context));
}
