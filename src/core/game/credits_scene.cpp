#include "game_scene_creators.h"
#include "game_scene_utils.h"
#include <utility>

struct CreditsSceneContext {
	std::function<void()> haltChannels;
	std::function<void(const std::string&, int)> playMusic;
	bool& hardMode;
	Cuadrado& credits;
	Avestruz& ostrich;
	Cuadrado& continueCard;
	std::function<void(Escena)> changeScene;
	bool& isClicking;
	Button& soundButton;
	SDL_Rect* mouse;
	std::function<void()> toggleMute;
};

class CreditsScene final : public GameScene {
	CreditsSceneContext context;
	public:
		explicit CreditsScene(CreditsSceneContext context) : context(std::move(context)) {}
		Escena id() const override { return CREDITS; }

		void enter(Escena) override {
			context.haltChannels();
			context.playMusic("Creditos", 1);
		}

		void exit(Escena) override {
			context.hardMode = false;
		}

		void handleClick() override {
			if (beginClick(context.isClicking, context.soundButton, context.mouse, context.toggleMute)) {
				context.isClicking = false;
			}
		}

		void update() override {
			if (!creditsShown) {
				context.credits.update(0, -1);
				if (context.credits.dstRect->y < -context.credits.dstRect->h) {
					creditsShown = true;
				}
			}
			if (!ostrichShown && creditsShown) {
				context.ostrich.update(0, -1);
				if (context.ostrich.dstRect->y < -context.ostrich.dstRect->h) {
					ostrichShown = true;
				}
			}
			if (ostrichShown && creditsShown) {
				context.continueCard.update(0, -1);
				if (context.continueCard.dstRect->y < -context.continueCard.dstRect->h) {
					creditsShown = false;
					ostrichShown = false;
					context.changeScene(MENU);
				}
			}
		}

		void render() override {
			context.credits.draw();
			context.ostrich.draw();
			if (SDL_GetTicks() / 16 % 20 == 0) {
				context.ostrich.animateX();
			}
			context.continueCard.draw();
		}

	private:
		bool creditsShown = false;
		bool ostrichShown = false;
	};

std::unique_ptr<GameScene> createCreditsScene(const GameSceneFactoryContext& bindings) {
	return std::make_unique<CreditsScene>(CreditsSceneContext{
		bindings.haltChannels,
		bindings.playMusic,
		bindings.hardMode,
		bindings.credits,
		bindings.ostrich,
		bindings.continueCard,
		bindings.changeScene,
		bindings.isClicking,
		bindings.soundButton,
		bindings.mouse,
		bindings.toggleMute
	});
}
