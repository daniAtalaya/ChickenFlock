#include "menu_scene_factory.h"
#include "game/game_scene_utils.h"
#include <utility>

class MenuScene final : public GameScene {
	MenuSceneContext context;
	Button creditsButton;
	Button soundButton;
	Button shopButton;
	Button hardcoreButton;
	SDL_Rect graphicsRoomButton{ 420, 755, 120, 95 };
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
			if (event.type != SDL_KEYDOWN || event.key.repeat) {
				return;
			}
			const SDL_Keycode key = event.key.keysym.sym;
			if (isConfirmKey(key)) {
				context.changeScene(LORE);
			}
			if (key == SDLK_t) {
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
			if (SDL_PointInRect(&position, &graphicsRoomButton)) {
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
			SDL_QueryTexture(context.assets.images.get("start"), nullptr, nullptr, &width, &height);
			renderTexture(renderer, context.assets.images.get("start"), {
				WINDOW_W - 100 - width / 3, (WINDOW_H / 2) - (height * 4 / 10) / 2,
				width * 1 / 3, height * 4 / 10
			});
			SDL_QueryTexture(context.assets.images.get("tituloCockFlock"), nullptr, nullptr, &width, &height);
			renderTexture(renderer, context.assets.images.get("tituloCockFlock"), {
				(WINDOW_W / 2) - 160, 50, width / 2, height / 2
			});
			creditsButton.draw(renderer, showHitboxes);
			renderCurrencyPanel(renderer, context.assets, context.progress.rupees);
			SDL_SetRenderDrawColor(renderer, 255, 250, 231, 255);
			SDL_RenderFillRect(renderer, &graphicsRoomButton);
			SDL_SetRenderDrawColor(renderer, 107, 137, 95, 255);
			SDL_RenderDrawRect(renderer, &graphicsRoomButton);
			drawPixelText(renderer, "F4", 458, 767, 4, { 44, 67, 53, 255 });
			drawPixelText(renderer, "GALLERY", 439, 821, 2, { 78, 100, 74, 255 });
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
