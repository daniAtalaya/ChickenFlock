#include "lore_scene_factory.h"
#include "game/game_scene_utils.h"
#include <array>
#include <utility>

namespace {
	constexpr std::array<const char*, 13> dialogue{{
		"Damn fences? They never work, those roosters just escaped again.",
		"I'm starting to desperate. Lady, bring the horses?",
		"I hope they are not harming anyone.",
		"The wild ostrich, what a savage animal. Happy she didn't see me.",
		"Everything is messed up, and broken... Marie isn't here anymore.",
		"It's been a whole week. No food. No water... No roosters.",
		"My time has come, at least farmer. Father was right.",
		"Maybe she finally left me. An old man, and an old disaster.",
		"I'm here again. Those animals of the devil will not return I'm afraid. Wait... what happened here?",
		"Is that a note in the wall?",
		"Go to the north to rescue this talker old woman, bring your roosters with you for barter.",
		"She didn't leave me, but I have no animals to barter.",
		"Hopefully I will find them on the way."
	}};
}

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
			if (event.type == SDL_EVENT_KEY_DOWN && !event.key.repeat && isConfirmKey(event.key.key)) {
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
			context.camera.draw(renderer, showHitboxes);
			context.player.draw(renderer, showHitboxes);
			soundButton.img = context.assets.images.get(context.muted ? "soundOff" : "soundOn");
			soundButton.draw(renderer, showHitboxes);
			renderHearts(renderer, context.player, showHitboxes);
			if (loreShown == 0) loreShown = 1;
			const SDL_Rect card{ (WINDOW_W - 448) / 2, WINDOW_H - 218, 448, 168 };
			renderTexture(renderer, context.assets.images.get("lore1"), card);
			SDL_SetRenderDrawColor(renderer, 255, 239, 190, 255);
			const SDL_Rect textMask{ card.x + 146, card.y + 17, 292, 135 };
			fillRect(renderer, textMask);
			drawWrappedPixelText(renderer, dialogue[static_cast<std::size_t>(loreShown - 1)],
				{ textMask.x + 6, textMask.y + 5, textMask.w - 14, textMask.h - 10 },
				2, { 35, 27, 20, 255 });
		}

	private:
		int loreShown = 0;
	};

std::unique_ptr<GameScene> createLoreScene(LoreSceneContext context) {
	return std::make_unique<LoreScene>(std::move(context));
}
