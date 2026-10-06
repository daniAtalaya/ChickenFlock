#include "lore_scene_factory.h"
#include "game/game_scene_utils.h"
#include <utility>

class LoreScene final : public GameScene {
	LoreSceneContext context;
	Button soundButton;
	public:
		explicit LoreScene(LoreSceneContext context) : context(std::move(context)) {
			assignRect(soundButton.dstRect, { 10, 20, 100, 100 });
		}
		Escena id() const override { return LORE; }

		void enter(Escena) override {
			context.playSound("SStart", 0);
			if (++loreShown > 13) {
				loreShown = 1;
			}
		}

		void handleInput(const SDL_Event& event) override {
			if (event.type == SDL_KEYDOWN && !event.key.repeat && isConfirmKey(event.key.keysym.sym)) {
				context.changeScene(JOC);
			}
		}

		void handleClick(const SDL_Point& position) override {
			if (soundButton.isClicked(position)) {
				context.toggleMute();
			} else {
				context.changeScene(JOC);
			}
		}

		void render(SDL_Renderer* renderer, const bool showHitboxes) override {
			int width, height;
			context.camera.draw(renderer, showHitboxes);
			context.player.draw(renderer, showHitboxes);
			soundButton.img = context.assets.images.get(context.muted ? "soundOff" : "soundOn");
			soundButton.draw(renderer, showHitboxes);
			renderHearts(renderer, context.player, showHitboxes);
			if (loreShown == 0) loreShown = 1;
			const std::string image = "lore" + std::to_string(loreShown);
			SDL_QueryTexture(context.assets.images.get(image), nullptr, nullptr, &width, &height);
			renderTexture(renderer, context.assets.images.get(image), {
				(WINDOW_W / 2) - (width * 7 / 5) / 2, WINDOW_H - height - 50,
				width * 7 / 5, height * 12 / 10
			});
		}

	private:
		int loreShown = 0;
	};

std::unique_ptr<GameScene> createLoreScene(LoreSceneContext context) {
	return std::make_unique<LoreScene>(std::move(context));
}
