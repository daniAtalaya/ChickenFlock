#include "game_scene_creators.h"
#include "game_scene_utils.h"
#include <utility>

struct GameplaySceneContext {
	SDL_Renderer* renderer;
	GameAssets& assets;
	std::function<void(const std::string&, int)> playMusic;
	bool& muted;
	std::function<void(const std::string&, int)> playSound;
	bool& paused;
	bool& hardMode;
	Cuadrado& horda;
	GameWorld& world;
	Player& player;
	Camera& camera;
	Pajaro& bird;
	Cuadrado& leftWall;
	Cuadrado& rightWall;
	const Uint8* keyboard;
	bool& godMode;
	int& gamesPlayed;
	int& temporaryMoney;
	std::function<void(Escena)> changeScene;
	bool& isClicking;
	Button& soundButton;
	SDL_Rect* mouse;
	std::function<void()> toggleMute;
	Button& playButton;
};

class GameplayScene final : public GameScene {
	GameplaySceneContext context;
	public:
		explicit GameplayScene(GameplaySceneContext context) : context(std::move(context)) {}
		Escena id() const override { return JOC; }

		void enter(Escena) override {
			context.playMusic("Gameplay", -1);
			if (!context.muted) {
				context.playSound("MultitudG", -1);
			}
			context.paused = false;
			if (context.hardMode) {
				context.horda.dstRect->y = WINDOW_H - context.horda.dstRect->h * 3;
			}
		}

		void exit(Escena nextScene) override {
			if (nextScene != PAUSA) {
				context.world.markForRemoval();
			}
		}

		void handleInput(const SDL_Event& event) override {
			if (event.type == SDL_KEYDOWN && !event.key.repeat
				&& event.key.keysym.sym == SDLK_SPACE
				&& (context.player.direccion == 1 || context.player.direccion == 3)) {
				context.world.shoot(context.player, context.assets, context.playSound);
			}
		}

		void handleClick() override {
			if (!beginClick(context.isClicking, context.soundButton, context.mouse, context.toggleMute)) {
				return;
			}
			context.world.collectBird(context.bird, context.mouse, context.gamesPlayed, context.temporaryMoney);
			context.isClicking = false;
		}

		void update() override {
			if (context.player.vides <= 0) {
				context.changeScene(GAMEOVER);
			}
			if (!context.paused) {
				if (context.camera.srcRect->y > 0) {
					context.camera.update();
				} else if (context.camera.sY != 0) {
					context.world.markForVictory();
					context.horda.dstRect->h = 0;
					context.player.money += context.temporaryMoney;
					context.changeScene(GUANYAT);
				}
				GameplayContext gameplayContext{
					context.player, context.camera, context.bird, context.horda,
					context.leftWall, context.rightWall, context.assets, context.keyboard,
					context.godMode, context.hardMode, context.gamesPlayed, context.temporaryMoney,
					context.changeScene, context.playSound
				};
				context.world.update(gameplayContext);
			}
		}

		void render() override {
			context.camera.sY = 2;
			context.camera.draw();
			context.soundButton.draw();
			context.playButton.draw();
			if ((SDL_GetTicks() / 16) % 20 == 0 && !context.paused) {
				context.player.animateX();
			}
			if (!context.paused) {
				context.player.animateY();
			}
			context.world.drawRupias();
			context.world.drawRocas();
			context.world.drawArboles();
			context.world.drawGallinas(context.paused);
			if (SDL_GetTicks() / 16 % 20 == 0 && !context.paused) {
				context.bird.animateX();
			}
			context.player.draw();
			context.leftWall.draw();
			context.rightWall.draw();
			renderHearts(context.player);
			renderTexture(context.renderer, context.assets.images.get("rupia1"), { WINDOW_W - 60, 90, 40, 40 });
			renderMoney(context.renderer, context.assets, context.temporaryMoney);
			context.horda.draw();
			if (context.hardMode) {
				for (int i = 1; i < 3; ++i) {
					renderTexture(context.renderer, context.assets.images.get("horda"), {
						context.horda.dstRect->x,
						WINDOW_H - context.horda.dstRect->h * i,
						context.horda.dstRect->w,
						context.horda.dstRect->h
					});
				}
			}
			context.world.drawFlechas();
			if (context.gamesPlayed % 2 == 0) {
				context.bird.draw();
				if ((SDL_GetTicks() / 16) % 20 == 0) context.bird.animateX();
			}
		}

	};

std::unique_ptr<GameScene> createGameplayScene(const GameSceneFactoryContext& bindings) {
	return std::make_unique<GameplayScene>(GameplaySceneContext{
		bindings.renderer,
		bindings.assets,
		bindings.playMusic,
		bindings.muted,
		bindings.playSound,
		bindings.paused,
		bindings.hardMode,
		bindings.horda,
		bindings.world,
		bindings.player,
		bindings.camera,
		bindings.bird,
		bindings.leftWall,
		bindings.rightWall,
		bindings.keyboard,
		bindings.godMode,
		bindings.gamesPlayed,
		bindings.temporaryMoney,
		bindings.changeScene,
		bindings.isClicking,
		bindings.soundButton,
		bindings.mouse,
		bindings.toggleMute,
		bindings.playButton
	});
}
