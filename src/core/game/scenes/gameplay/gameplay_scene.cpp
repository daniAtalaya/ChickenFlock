#include "gameplay_scene_factory.h"
#include "game/game_scene_utils.h"
#include "game/game_world.h"
#include "npc/pajaro/pajaro.h"
#include <utility>

class GameplayScene final : public GameScene {
	GameplaySceneContext context;
	Cuadrado horda;
	Cuadrado leftWall;
	Cuadrado rightWall;
	Pajaro bird;
	GameWorld world;
	Button soundButton;
	Button playButton;
	int temporaryRupees = 0;
	int hordeHeight = 0;
	public:
		explicit GameplayScene(GameplaySceneContext context) : context(std::move(context)) {
			horda.img = this->context.assets.images.get("horda");
			assignRect(horda.dstRect, { 130, WINDOW_H, 0, 0 });
			getTextureSize(horda.img, &horda.dstRect->w, &horda.dstRect->h);
			hordeHeight = horda.dstRect->h;
			resetHorde();
			assignRect(leftWall.dstRect, { 1, 1, 150, 8100 });
			assignRect(rightWall.dstRect, { 810, 1, 150, 8100 });
			bird.init(this->context.assets.images.get("pajaro"));
			assignRect(bird.dstRect, { -200, R_NUM(0, WINDOW_H - 200), 70, 100 });
			bird.sX = 8;
			bird.sY = R_NUM(-4, 4);
			soundButton.img = this->context.assets.images.get("soundOn");
			assignRect(soundButton.dstRect, { 10, 20, 100, 100 });
			playButton.img = this->context.assets.images.get("pause");
			assignRect(playButton.dstRect, { 10, 135, 100, 100 });
		}
		Escena id() const override { return JOC; }

		void enter(const Escena previousScene) override {
			world.cleanup();
			if (previousScene != PAUSA) {
				resetHorde();
				if (context.hardMode) {
					horda.dstRect->y = WINDOW_H - hordeHeight * 3;
				}
				context.playMusic("Gameplay", -1);
				context.playSound("MultitudG", -1);
			}
			if (previousScene == LORE) temporaryRupees = 0;
		}

		void exit(Escena nextScene) override {
			if (nextScene != PAUSA) {
				context.progress.rupees += temporaryRupees;
				temporaryRupees = 0;
				world.markForRemoval();
			}
		}

		void handleInput(const SDL_Event& event) override {
			if (event.type != SDL_EVENT_KEY_DOWN || event.key.repeat) return;
			if (event.key.key == SDLK_SPACE
				&& (context.player.direccion == 1 || context.player.direccion == 3)) {
				world.shoot(context.player, context.assets, context.playSound);
			}
		}

		void handleClick(const SDL_Point& position) override {
			if (soundButton.isClicked(position)) {
				context.toggleMute();
			}
			if (playButton.isClicked(position)) context.togglePause();
			if (!soundButton.isClicked(position) && !playButton.isClicked(position)) {
				SDL_Rect clickRect{ position.x, position.y, 1, 1 };
				world.collectBird(bird, &clickRect, context.progress.gamesPlayed, temporaryRupees);
			}
		}

		void update() override {
			world.cleanup();
			if (context.player.vides <= 0 && !context.debugHitboxes) {
				context.changeScene(GAMEOVER);
				return;
			}
			if (context.camera.srcRect->y > 0) {
				context.camera.update();
			} else if (context.camera.sY != 0) {
				world.markForVictory();
				context.changeScene(GUANYAT);
				return;
			}
			GameplayContext gameplayContext{
				context.player, context.camera, bird, horda,
				leftWall, rightWall, context.assets, context.keyboard,
				context.debugHitboxes, context.hardMode, context.progress, temporaryRupees,
				context.changeScene, context.playSound
			};
			world.update(gameplayContext);
		}

		void render(SDL_Renderer* renderer, const bool showHitboxes) override {
			const bool showGameplayDebug = showHitboxes;
			context.camera.sY = 2;
			context.camera.draw(renderer, showGameplayDebug);
			soundButton.img = context.assets.images.get(context.muted ? "soundOff" : "soundOn");
			soundButton.draw(renderer, showGameplayDebug);
			playButton.img = context.assets.images.get("pause");
			playButton.draw(renderer, showGameplayDebug);
			if ((SDL_GetTicks() / 16) % 20 == 0) {
				context.player.animateX();
			}
			context.player.animateY();
			world.drawRupias(renderer, showGameplayDebug);
			world.drawRocas(renderer, showGameplayDebug);
			world.drawArboles(renderer, showGameplayDebug);
			world.drawGallinas(renderer, showGameplayDebug);
			if (SDL_GetTicks() / 16 % 20 == 0) {
				bird.animateX();
			}
			const bool blink = context.player.isInvulnerable() && (SDL_GetTicks() / 100) % 2 == 0;
			SDL_SetTextureAlphaMod(context.player.img, blink ? 100 : 255);
			context.player.draw(renderer, showGameplayDebug);
			SDL_SetTextureAlphaMod(context.player.img, 255);
			leftWall.draw(renderer, showGameplayDebug);
			rightWall.draw(renderer, showGameplayDebug);
			renderHearts(renderer, context.player, showGameplayDebug);
			renderCurrencyPanel(renderer, context.assets, context.progress.rupees, temporaryRupees);
			horda.draw(renderer, showGameplayDebug);
			if (context.hardMode) {
				for (int i = 1; i < 3; ++i) {
					renderTexture(renderer, context.assets.images.get("horda"), {
						horda.dstRect->x,
						WINDOW_H - horda.dstRect->h * i,
						horda.dstRect->w,
						horda.dstRect->h
					});
				}
			}
			world.drawFlechas(renderer, showGameplayDebug);
			if (context.progress.gamesPlayed % 2 == 0) {
				bird.draw(renderer, showGameplayDebug);
				if ((SDL_GetTicks() / 16) % 20 == 0) bird.animateX();
			}
		}

	private:
		void resetHorde() {
			horda.dstRect->h = hordeHeight;
			horda.dstRect->y = WINDOW_H - hordeHeight;
		}

	};

std::unique_ptr<GameScene> createGameplayScene(GameplaySceneContext context) {
	return std::make_unique<GameplayScene>(std::move(context));
}
