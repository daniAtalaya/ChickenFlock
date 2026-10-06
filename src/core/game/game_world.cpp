#include "game_world.h"

#include <algorithm>
#include <string>

namespace {
	template <typename T>
	void destroyEntity(T* entity) {
		if (entity == nullptr) {
			return;
		}
		delete entity->srcRect;
		delete entity->dstRect;
		delete entity;
	}

	template <typename T>
	void removeExpired(std::vector<T*>& entities, bool arrows) {
		entities.erase(std::remove_if(entities.begin(), entities.end(), [arrows](T* entity) {
			const SDL_Rect* rectangle = entity->dstRect;
			const bool offscreen = rectangle->x < -rectangle->w
				|| rectangle->x > WINDOW_W + rectangle->w
				|| rectangle->y > WINDOW_H + rectangle->h
				|| (arrows && rectangle->y < -rectangle->h);
			if (entity->disposable || offscreen) {
				destroyEntity(entity);
				return true;
			}
			return false;
		}), entities.end());
	}

	template <typename T>
	void destroyAll(std::vector<T*>& entities) {
		for (T* entity : entities) {
			destroyEntity(entity);
		}
		entities.clear();
	}
}

GameWorld::~GameWorld() {
	clear();
}

void GameWorld::clear() {
	destroyAll(arboles);
	destroyAll(rocas);
	destroyAll(rupias);
	destroyAll(gallinas);
	destroyAll(flechas);
}

void GameWorld::cleanup() {
	removeExpired(arboles, false);
	removeExpired(rocas, false);
	removeExpired(rupias, false);
	removeExpired(gallinas, false);
	removeExpired(flechas, true);
}

void GameWorld::markForRemoval() {
	for (const Gallina* chicken : gallinas) chicken->disposable = true;
	for (const Rupia* rupee : rupias) rupee->disposable = true;
	for (const Flecha* arrow : flechas) arrow->disposable = true;
	for (const Roca* rock : rocas) rock->disposable = true;
	for (const Arbol* tree : arboles) tree->disposable = true;
}

void GameWorld::markForVictory() {
	for (const Gallina* chicken : gallinas) chicken->disposable = true;
	for (const Rupia* rupee : rupias) rupee->disposable = true;
}

void GameWorld::shoot(const Player& player, GameAssets& assets,
	const std::function<void(const std::string&, int)>& playSound) {
	if (player.direccion != 1 && player.direccion != 3) {
		return;
	}

	auto* arrow = new Flecha();
	arrow->img = assets.images.get("flecha");
	playSound("disparoFlecha", 0);
	if (player.direccion == 1) {
		arrow->sX = 0;
		arrow->sY = -5;
	}
	if (player.direccion == 3) {
		arrow->img = assets.images.get("flechab");
		arrow->sX = 0;
		arrow->sY = 6;
	}
	*arrow->dstRect = {
		player.dstRect->x + 15,
		player.direccion == 3 ? player.dstRect->y + player.dstRect->h + 5 : player.dstRect->y - 5,
		30,
		60
	};
	flechas.push_back(arrow);
}

bool GameWorld::collectBird(Pajaro& bird, const SDL_Rect* mouse, int playedGames, int& temporaryRupees) {
	if (playedGames % 2 != 0 || !bird.checkCollision(mouse)) {
		return false;
	}
	bird.dstRect->h = 0;
	temporaryRupees += 5;
	return true;
}

void GameWorld::movePlayer(GameplayContext& context) {
	Player& player = context.player;
	if constexpr (true) {
		if (context.keyboard[SDL_SCANCODE_W]) {
			player.direccion = 1;
			player.update(0, -1);
		}
		if (context.keyboard[SDL_SCANCODE_S]) {
			player.direccion = 3;
			player.update(0, 1);
		}
		if (context.keyboard[SDL_SCANCODE_A]) {
			player.direccion = 0;
			if (!player.checkCollision(context.leftWall.dstRect)) {
				player.update(-1, 0);
			}
		}
		if (context.keyboard[SDL_SCANCODE_D]) {
			player.direccion = 2;
			if (!player.checkCollision(context.rightWall.dstRect)) {
				player.update(1, 0);
			}
		}
	}
}

void GameWorld::updateBird(GameplayContext& context) {
	if (context.progress.gamesPlayed % 2 != 0) {
		return;
	}
	Pajaro& bird = context.bird;
	bird.update();
	if (bird.dstRect->x > WINDOW_W) {
		bird.sX = -8;
		bird.sY = R_NUM(-4, 4);
		bird.dstRect->y = R_NUM(0, WINDOW_H - 200);
	}
	if (bird.dstRect->x < -bird.dstRect->w) {
		bird.sX = 8;
		bird.sY = R_NUM(-4, 4);
		bird.dstRect->y = R_NUM(0, WINDOW_H - 200);
	}
}

void GameWorld::spawn(GameplayContext& context) {
	if (context.camera.srcRect->y <= 900) {
		return;
	}

	Camera& camera = context.camera;
	GameAssets& assets = context.assets;
	const auto randomX = [&context]() {
		return R_NUM(context.leftWall.dstRect->w, WINDOW_W - (context.rightWall.dstRect->w * 2));
	};
	const auto makeChicken = [&assets](int type, int x, int y) {
		auto* chicken = new Gallina();
		chicken->tipus = type;
		*chicken->dstRect = { x, y, 40, 40 };
		chicken->init(assets.images.get("gallina" + std::to_string(chicken->tipus)));
		return chicken;
	};

	if (SDL_GetTicks() / 16 % 300 == 0) {
		for (int i = 0; i < 2; i++) {
			auto* rupee = new Rupia();
			rupee->tipus = 1;
			rupee->valor = 1;
			rupee->img = assets.images.get("rupia" + std::to_string(rupee->tipus));
			*rupee->dstRect = { randomX(), R_NUM(-250, -50), 55, 55 };
			rupias.push_back(rupee);
		}
	}
	if (SDL_GetTicks() / 16 % 450 == 0) {
		for (int i = 0; i < 1; i++) {
			auto* rupee = new Rupia();
			rupee->tipus = R_NUM(2, 4);
			rupee->valor = 2;
			rupee->img = assets.images.get("rupia" + std::to_string(rupee->tipus));
			*rupee->dstRect = { randomX(), R_NUM(-250, -50), 55, 55 };
			rupias.push_back(rupee);
		}
	}
	if (SDL_GetTicks() / 16 % 150 == 0) {
		const auto spawnChicken = [&randomX, &makeChicken](int type) {
			const int x = randomX();
			const int y = R_NUM(-250, -50);
			return makeChicken(type, x, y);
		};
		gallinas.push_back(spawnChicken(1));
		if (context.progress.brownChickenUnlocked) gallinas.push_back(spawnChicken(2));
		if (context.progress.blueChickenUnlocked) gallinas.push_back(spawnChicken(3));
		if (context.progress.darkChickenUnlocked) gallinas.push_back(spawnChicken(4));
		if (context.progress.goldenChickenUnlocked) gallinas.push_back(spawnChicken(5));
	}
	if (SDL_GetTicks() / 16 % 100 == 0) {
		tipoGallinaTrasera = R_NUM(1, 5);
		while (!context.progress.isChickenUnlocked(tipoGallinaTrasera)) --tipoGallinaTrasera;
		const int x = randomX();
		const int y = WINDOW_H + R_NUM(-150, -50);
		auto* chicken = makeChicken(tipoGallinaTrasera, x, y);
		chicken->sY = -5;
		gallinas.push_back(chicken);
	}
	if (SDL_GetTicks() / 16 % 300 == 0) {
		for (int i = 0; i <= R_NUM(0, 1); i++) {
			auto* tree = new Arbol();
			tree->sX = 0;
			tree->sY = camera.sY;
			tree->img = assets.images.get("arbol" + std::to_string(R_NUM(1, 4)));
			*tree->dstRect = { randomX(), -150 * R_NUM(1, 3), 40, 40 };
			getTextureSize(tree->img, &tree->dstRect->w, &tree->dstRect->h);
			tree->dstRect->w *= (35 / 10);
			tree->dstRect->h *= (35 / 10);
			arboles.push_back(tree);
		}
	}
	if (SDL_GetTicks() / 16 % 250 == 0) {
		for (int i = 0; i <= R_NUM(0, 1); i++) {
			auto* rock = new Roca();
			rock->sX = 0;
			rock->sY = camera.sY;
			rock->img = assets.images.get("roca" + std::to_string(R_NUM(1, 4)));
			*rock->dstRect = { randomX(), R_NUM(-450, -150), 40, 40 };
			rock->dstRect->x *= (15 / 10);
			rock->dstRect->y *= (15 / 10);
			rocas.push_back(rock);
		}
	}
}

void GameWorld::resolveCollisions(GameplayContext& context) {
	Player& player = context.player;
	for (const Rupia* rupee : rupias) {
		rupee->update(0, 1);
		if (player.checkCollision(rupee->dstRect)) {
			rupee->disposable = true;
			context.playSound("SMoneda", 0);
			context.temporaryRupees += rupee->valor;
		}
	}
	for (const Gallina* chicken : gallinas) {
		chicken->update();
		if (player.checkCollision(chicken->dstRect)) {
			chicken->disposable = true;
			if (!context.godMode && player.damage()) {
				context.playSound(u8"dañoGallina", 0);
			}
		}
	}
	for (const Roca* rock : rocas) {
		rock->update();
		if (player.checkCollision(rock->dstRect)) {
			if (!context.godMode) {
				context.playSound(u8"dañoQueja", 0);
				if (!context.hardMode) context.changeScene(GAMEOVER);
			}
		}
	}
	for (const Arbol* tree : arboles) {
		tree->update();
		if (player.checkCollision(tree->dstRect) && !context.hardMode && !context.godMode) {
			context.playSound(u8"dañoQueja", 0);
			if (!context.hardMode) context.changeScene(GAMEOVER);
		}
	}
	for (const Flecha* arrow : flechas) {
		arrow->update();
		for (const Gallina* chicken : gallinas) {
			if (arrow->checkCollision(chicken->dstRect)) {
				context.playSound("muerteGallina", 0);
				chicken->disposable = true;
				arrow->disposable = true;
				context.temporaryRupees += R_NUM(0, chicken->tipus * 2);
			}
		}
	}
}

void GameWorld::update(GameplayContext& context) {
	movePlayer(context);
	updateBird(context);
	if (context.player.checkCollision(context.horda.dstRect)) {
		const bool damaged = !context.godMode && context.player.damage();
		context.player.dstRect->y -= context.horda.dstRect->h + 10;
		if (damaged) {
			context.playSound(u8"dañoGallina", 0);
		}
	}
	spawn(context);
	resolveCollisions(context);
}

void GameWorld::drawRupias(SDL_Renderer* renderer, const bool showHitboxes) const {
	for (const Rupia* rupee : rupias) rupee->draw(renderer, showHitboxes);
}

void GameWorld::drawRocas(SDL_Renderer* renderer, const bool showHitboxes) const {
	for (const Roca* rock : rocas) rock->draw(renderer, showHitboxes);
}

void GameWorld::drawArboles(SDL_Renderer* renderer, const bool showHitboxes) const {
	for (const Arbol* tree : arboles) tree->draw(renderer, showHitboxes);
}

void GameWorld::drawGallinas(SDL_Renderer* renderer, const bool showHitboxes) {
	for (Gallina* chicken : gallinas) {
		chicken->draw(renderer, showHitboxes);
		if (SDL_GetTicks() / 16 % 20 == 0) chicken->animateX();
		if (SDL_GetTicks() / 16 % 200 * chicken->spritesheet.maxC == 0) chicken->animateY();
	}
}

void GameWorld::drawFlechas(SDL_Renderer* renderer, const bool showHitboxes) const {
	for (const Flecha* arrow : flechas) arrow->draw(renderer, showHitboxes);
}
