#include "game_scene_creators.h"

std::unique_ptr<GameScene> createGameScene(Escena scene, const GameSceneFactoryContext& context) {
	switch (scene) {
	case INICI: return createIntroScene(context);
	case MENU: return createMenuScene(context);
	case LORE: return createLoreScene(context);
	case JOC: return createGameplayScene(context);
	case GAMEOVER: return createGameOverScene(context);
	case GUANYAT: return createVictoryScene(context);
	case TIENDA: return createShopScene(context);
	case PAUSA: return createPauseScene(context);
	case CREDITS: return createCreditsScene(context);
	default: return createIntroScene(context);
	}
}
