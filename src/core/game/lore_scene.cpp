#include "game_scene_creators.h"
#include "game_scene_utils.h"
#include <utility>

struct LoreSceneContext {
	SDL_Renderer* renderer;
	GameAssets& assets;
	Escena& currentScene;
	bool& muted;
	std::function<void(const std::string&, int)> playSound;
	std::function<void(Escena)> changeScene;
	bool& isClicking;
	Button& soundButton;
	SDL_Rect* mouse;
	std::function<void()> toggleMute;
	GameWorld& world;
	Pajaro& bird;
	int& gamesPlayed;
	int& temporaryMoney;
	Camera& camera;
	Player& player;
};

class LoreScene final : public GameScene {
	LoreSceneContext context;
	public:
		explicit LoreScene(LoreSceneContext context) : context(std::move(context)) {}
		Escena id() const override { return LORE; }

		void enter(Escena) override {
			if (!context.muted) {
				context.playSound("SStart", 0);
			}
			if (++loreShown > 13) {
				loreShown = 1;
			}
		}

		void handleInput(const SDL_Event& event) override {
			if (event.type == SDL_KEYDOWN && !event.key.repeat && isConfirmKey(event.key.keysym.sym)) {
				context.changeScene(JOC);
			}
		}

		void handleClick() override {
			if (!beginClick(context.isClicking, context.soundButton, context.mouse, context.toggleMute)) {
				return;
			}
			if (!context.soundButton.isClicked(context.mouse) && context.currentScene == LORE) {
				context.changeScene(JOC);
			}
			if (context.currentScene == JOC) {
				context.world.collectBird(context.bird, context.mouse, context.gamesPlayed, context.temporaryMoney);
			}
			context.isClicking = false;
		}

		void render() override {
			int width, height;
			context.camera.draw();
			context.player.draw();
			context.soundButton.draw();
			renderHearts(context.player);
			if (loreShown == 0) loreShown = 1;
			const std::string image = "lore" + std::to_string(loreShown);
			SDL_QueryTexture(context.assets.images.get(image), nullptr, nullptr, &width, &height);
			renderTexture(context.renderer, context.assets.images.get(image), {
				(WINDOW_W / 2) - (width * 7 / 5) / 2, WINDOW_H - height - 50,
				width * 7 / 5, height * 12 / 10
			});
		}

	private:
		int loreShown = 0;
	};

std::unique_ptr<GameScene> createLoreScene(const GameSceneFactoryContext& bindings) {
	return std::make_unique<LoreScene>(LoreSceneContext{
		bindings.renderer,
		bindings.assets,
		bindings.currentScene,
		bindings.muted,
		bindings.playSound,
		bindings.changeScene,
		bindings.isClicking,
		bindings.soundButton,
		bindings.mouse,
		bindings.toggleMute,
		bindings.world,
		bindings.bird,
		bindings.gamesPlayed,
		bindings.temporaryMoney,
		bindings.camera,
		bindings.player
	});
}
