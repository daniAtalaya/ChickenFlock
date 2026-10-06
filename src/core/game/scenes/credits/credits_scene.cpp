#include "credits_scene_factory.h"
#include "game/game_scene_utils.h"
#include <utility>

class CreditsScene final : public GameScene {
	CreditsSceneContext context;
	Button soundButton;
	Cuadrado credits;
	Avestruz ostrich;
	Cuadrado continueCard;
	public:
		explicit CreditsScene(CreditsSceneContext context) : context(std::move(context)) {
			credits.img = this->context.assets.images.get("creditos");
			credits.sY = 2;
			assignRect(credits.dstRect, { 75, (WINDOW_H * 15 / 10), WINDOW_W - 150, WINDOW_H * 16 / 10 });
			ostrich.sY = 1;
			ostrich.init(this->context.assets.images.get("avestruz"));
			assignRect(ostrich.dstRect, { WINDOW_H / 2 - 100, (WINDOW_H * 15 / 10), 200, 200 });
			continueCard.img = this->context.assets.images.get("continuara");
			continueCard.sY = 6;
			assignRect(continueCard.dstRect, { WINDOW_H / 2 - 325, (WINDOW_H * 15 / 10), 700, 450 });
			assignRect(soundButton.dstRect, { 10, 20, 100, 100 });
		}
		Escena id() const override { return CREDITS; }

		void enter(Escena) override {
			credits.dstRect->y = WINDOW_H * 15 / 10;
			ostrich.dstRect->y = WINDOW_H * 15 / 10;
			continueCard.dstRect->y = WINDOW_H * 15 / 10;
			creditsShown = false;
			ostrichShown = false;
			context.haltChannels();
			context.playMusic("Creditos", 1);
		}

		void handleClick(const SDL_Point& position) override {
			if (soundButton.isClicked(position)) context.toggleMute();
		}

		void update() override {
			if (!creditsShown) {
				credits.update(0, -1);
				if (credits.dstRect->y < -credits.dstRect->h) {
					creditsShown = true;
				}
			}
			if (!ostrichShown && creditsShown) {
				ostrich.update(0, -1);
				if (ostrich.dstRect->y < -ostrich.dstRect->h) {
					ostrichShown = true;
				}
			}
			if (ostrichShown && creditsShown) {
				continueCard.update(0, -1);
				if (continueCard.dstRect->y < -continueCard.dstRect->h) {
					creditsShown = false;
					ostrichShown = false;
					context.changeScene(MENU);
				}
			}
		}

		void render(SDL_Renderer* renderer, const bool showHitboxes) override {
			credits.draw(renderer, showHitboxes);
			ostrich.draw(renderer, showHitboxes);
			soundButton.img = context.assets.images.get(context.muted ? "soundOff" : "soundOn");
			soundButton.draw(renderer, showHitboxes);
			if (SDL_GetTicks() / 16 % 20 == 0) {
				ostrich.animateX();
			}
			continueCard.draw(renderer, showHitboxes);
		}

	private:
		bool creditsShown = false;
		bool ostrichShown = false;
	};

std::unique_ptr<GameScene> createCreditsScene(CreditsSceneContext context) {
	return std::make_unique<CreditsScene>(std::move(context));
}
