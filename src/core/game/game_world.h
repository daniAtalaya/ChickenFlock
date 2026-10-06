#pragma once

#include "enums.h"
#include "player/player.h"
#include "gameplay/camera.h"
#include "npc/pajaro/pajaro.h"
#include "resources/game_assets.h"
#include "economy/rupia.h"
#include "enemies/arbol/arbol.h"
#include "enemies/gallina/gallina.h"
#include "enemies/roca/roca.h"
#include "player/weapon/flecha.h"
#include "game_progress.h"

#include <functional>
#include <string>
#include <vector>

struct GameplayContext {
	Player& player;
	Camera& camera;
	Pajaro& bird;
	Cuadrado& horda;
	const Cuadrado& leftWall;
	const Cuadrado& rightWall;
	GameAssets& assets;
	const bool* keyboard;
	bool godMode;
	bool hardMode;
	GameProgress& progress;
	int& temporaryRupees;
	std::function<void(Escena)> changeScene;
	std::function<void(const std::string&, int)> playSound;
};

class GameWorld {
public:
	GameWorld() = default;
	GameWorld(const GameWorld&) = delete;
	GameWorld& operator=(const GameWorld&) = delete;
	GameWorld(GameWorld&&) = delete;
	GameWorld& operator=(GameWorld&&) = delete;
	~GameWorld();

	void update(GameplayContext& context);
	void shoot(const Player& player, GameAssets& assets,
	const std::function<void(const std::string&, int)>& playSound);
	bool collectBird(Pajaro& bird, const SDL_Rect* mouse, int playedGames, int& temporaryRupees);
	void cleanup();
	void markForRemoval();
	void markForVictory();
	void clear();

	void drawRupias(SDL_Renderer* renderer, bool showHitboxes) const;
	void drawRocas(SDL_Renderer* renderer, bool showHitboxes) const;
	void drawArboles(SDL_Renderer* renderer, bool showHitboxes) const;
	void drawGallinas(SDL_Renderer* renderer, bool showHitboxes);
	void drawFlechas(SDL_Renderer* renderer, bool showHitboxes) const;

private:
	void spawn(GameplayContext& context);
	void movePlayer(GameplayContext& context);
	void updateBird(GameplayContext& context);
	void resolveCollisions(GameplayContext& context);

	std::vector<Rupia*> rupias;
	std::vector<Gallina*> gallinas;
	std::vector<Arbol*> arboles;
	std::vector<Roca*> rocas;
	std::vector<Flecha*> flechas;
	int tipoGallinaTrasera = 0;
};
