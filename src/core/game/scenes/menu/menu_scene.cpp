#include "menu_scene_factory.h"
#include "game/game_scene_utils.h"
#include <utility>

class MenuScene final : public GameScene {
	MenuSceneContext context;
	Button creditsButton;
	Button soundButton;
	Button shopButton;
	Button hardcoreButton;
	SDL_Rect graphicsRoomButton{ 405, 770, 150, 60 };
	Perro pet;
	public:
		explicit MenuScene(MenuSceneContext context) : context(std::move(context)) {
			creditsButton.img = this->context.assets.images.get("creditosBoton");
			assignRect(creditsButton.dstRect, { WINDOW_W - 410, 750, 330, 100 });
			soundButton.img = this->context.assets.images.get("soundOn");
			assignRect(soundButton.dstRect, { 10, 20, 100, 100 });
			shopButton.img = this->context.assets.images.get("tienda");
			assignRect(shopButton.dstRect, { 10, 135, 100, 100 });
			hardcoreButton.img = this->context.assets.images.get("hardcore");
			assignRect(hardcoreButton.dstRect, { (WINDOW_W / 2) - 410, 750, 330, 100 });
			pet.init(this->context.assets.images.get("mascota"));
			assignRect(pet.dstRect, { (WINDOW_W / 2) + 150, 75, 200, 200 });
		}
		Escena id() const override { return MENU; }

		void enter(Escena) override {
			context.haltChannels();
			context.playMusic("Menu", -1);
		}

		void exit(Escena) override {
			context.initialize();
		}

		void handleInput(const SDL_Event& event) override {
			if (event.type != SDL_EVENT_KEY_DOWN || event.key.repeat) {
				return;
			}
			const SDL_Keycode key = event.key.key;
			if (isConfirmKey(key)) {
				context.changeScene(LORE);
			}
			if (key == SDLK_T) {
				context.changeScene(TIENDA);
			}
		}

		void handleClick(const SDL_Point& position) override {
			if (soundButton.isClicked(position)) context.toggleMute();
			if (creditsButton.isClicked(position)) {
				context.changeScene(CREDITS);
				return;
			}
			if (shopButton.isClicked(position)) {
				context.changeScene(TIENDA);
				return;
			}
			if (hardcoreButton.isClicked(position) && !context.hardMode) {
				context.hardMode = !context.hardMode;
				return;
			}
			if (context.debugHitboxes && SDL_PointInRect(&position, &graphicsRoomButton)) {
				context.changeScene(GRAPHICS_ROOM);
				return;
			}
			if (!soundButton.isClicked(position)) context.changeScene(LORE);
		}

		void render(SDL_Renderer* renderer, const bool showHitboxes) override {
			int width, height;
			context.camera.sY = 0;
			context.camera.draw(renderer, showHitboxes);
			context.player.draw(renderer, showHitboxes);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
			fillRect(renderer, { 0, 0, WINDOW_W, WINDOW_H });
			soundButton.img = context.assets.images.get(context.muted ? "soundOff" : "soundOn");
			soundButton.draw(renderer, showHitboxes);
			shopButton.draw(renderer, showHitboxes);
			getTextureSize(context.assets.images.get("start"), &width, &height);
			renderTexture(renderer, context.assets.images.get("start"), {
				WINDOW_W - 100 - width / 3, (WINDOW_H / 2) - (height * 4 / 10) / 2,
				width * 1 / 3, height * 4 / 10
			});
			getTextureSize(context.assets.images.get("tituloCockFlock"), &width, &height);
			renderTexture(renderer, context.assets.images.get("tituloCockFlock"), {
				(WINDOW_W / 2) - 160, 50, width / 2, height / 2
			});
			creditsButton.draw(renderer, showHitboxes);
			renderCurrencyPanel(renderer, context.assets, context.progress.rupees);
			if (context.debugHitboxes) {
				SDL_SetRenderDrawColor(renderer, 255, 239, 190, 255);
				fillRect(renderer, graphicsRoomButton);
				SDL_SetRenderDrawColor(renderer, 127, 82, 38, 255);
				drawRect(renderer, graphicsRoomButton);
				const std::string label = "GRAPHICS ROOM";
				const int labelScale = 1;
				drawPixelText(renderer, label,
					graphicsRoomButton.x + (graphicsRoomButton.w - pixelTextWidth(label, labelScale)) / 2,
					graphicsRoomButton.y + (graphicsRoomButton.h - 7 * labelScale) / 2,
					labelScale, { 44, 67, 53, 255 });
			}
			if (!context.hardMode) {
				hardcoreButton.draw(renderer, showHitboxes);
			}
			if (context.progress.goldenChickenUnlocked) {
				pet.draw(renderer, showHitboxes);
				if ((SDL_GetTicks() / 16) % 20 == 0) pet.animateX();
				if ((SDL_GetTicks() / 16) % (100 * pet.spritesheet.maxC) == 0) {
					pet.animateY();
				}
			}
		}
	};

std::unique_ptr<GameScene> createMenuScene(MenuSceneContext context) {
	return std::make_unique<MenuScene>(std::move(context));
}
