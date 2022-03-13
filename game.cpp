
#pragma once
#include "game.h"
#include "general.h"
#include "color.h"
#include <filesystem>
#include <sstream>
#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
            ( std::ostringstream() << std::dec << x ) ).str()
Game::Game() {
	INIT_R;
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow(
		"Cock Flock",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_W, WINDOW_H,
		SDL_WINDOW_HIDDEN
	);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) return;
	if (Mix_Init(MIX_INIT_OGG) != MIX_INIT_OGG) return;
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) return;
	keyboard = SDL_GetKeyboardState(NULL);
	SDL_RenderSetScale(renderer, 1, 1);
	//for (const auto& entry : std::filesystem::directory_iterator("./sprites")) std::cout << entry.path() << std::endl;
	isOpen = load();
	assignImg();
	init();
	SDL_ShowWindow(window);
	SDL_GetMouseState(&mouseX, &mouseY);
	//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
}

void Game::assignImg() {
	horda.img = images.get("horda");
	camera.img = images.get("mapa3");
	botonBack.img = images.get("back");
	botonShop.img = images.get("tienda");
}

void Game::init() {
	botonPlay.img = images.get("play");
	paredHitboxLeft = Cuadrado();
	paredHitboxRight = Cuadrado();
	paredHitboxLeft.dstRect = new SDL_Rect({ 1, 1, 150, 8100 });
	paredHitboxRight.dstRect = new SDL_Rect({ 810, 1, 150, 8100 });
	//hitboxes.push_back(Cuadrado());
	//hitboxes[hitboxes.size() - 1].dstRect = new SDL_Rect({ 150, 200, 210, 300 });
	//hitboxes.push_back(Cuadrado());
	//hitboxes[hitboxes.size() - 1].dstRect = new SDL_Rect({ 810, 200, 210, 300 });
	botonSonido.img = images.get("soundOn");
	botonSonido.dstRect = new SDL_Rect({ 20, 20, 100, 100 });
	botonPlay.dstRect = new SDL_Rect({ 10, 135, 100, 100 });
	botonBack.dstRect = new SDL_Rect({ 10, 135, 100, 100 });
	botonShop.dstRect = new SDL_Rect({ 10, 135, 100, 100 });
	nivel.dstRect = new SDL_Rect({ 0, 0, WINDOW_W, 0 });
	player.dstRect = new SDL_Rect({ (WINDOW_W / 2) - 42, WINDOW_H - 300 , 50, 50 });
	SDL_QueryTexture(images.get("mapa3"), NULL, NULL, NULL, &nivel.dstRect->h);
	camera.srcRect = new SDL_Rect({ 0, nivel.dstRect->h - WINDOW_H, WINDOW_W, WINDOW_H });
	horda.dstRect = new SDL_Rect({ 130, WINDOW_H, 0, 0 });
	player.init(images.get("link"));
	SDL_QueryTexture(images.get("horda"), NULL, NULL, &horda.dstRect->w, &horda.dstRect->h);
	horda.dstRect->y = WINDOW_H - horda.dstRect->h;
	player.vides = 3;
	for (int i = 0; i < 3; i++) {
		player.corazones[i].img = images.get("corazon");
		player.corazones[i].alive = player.corazones[i].img;
		player.corazones[i].dead = images.get("corazont");
	}

}

bool Game::load() {
	//sfxs
	if (!tracks.load("Intro", "Intro Colibri Studios.ogg")) return false;
	if (!tracks.load("Game Over", "Game Over.ogg")) return false;
	if (!tracks.load("Gameplay", "Gameplay.ogg")) return false;
	if (!tracks.load("Menu", "Menu.ogg")) return false;
	if (!tracks.load("Tienda", "Tienda.ogg")) return false;
	if (!tracks.load("sonido de start", "sonido de start.ogg")) return false;
	if (!tracks.load("Multidud de gallinas", "Multidud de gallinas.ogg")) return false;
	if (!images.load("soundOn", "soundOn.png")) return false;
	if (!images.load("soundOff", "soundOff.png")) return false;
	if (!images.load("mapa3", "mapa3.png")) return false;
	if (!images.load("play", "play.png")) return false;
	if (!images.load("link", "link.png")) return false;
	if (!images.load("rupia", "rupia.png")) return false;
	if (!images.load("pajaro", "pajaro.png")) return false;
	if (!images.load("corazon", "corazon.png")) return false;
	if (!images.load("corazont", "corazont.png")) return false;
	if (!images.load("enter", "press_enter.png")) return false;
	if (!images.load("pause", "pause.png")) return false;
	if (!images.load("studio", "studio.png")) return false;
	if (!images.load("horda", "horda.png")) return false;
	if (!images.load("flecha", "flecha.png")) return false;
	if (!images.load("start", "start.png")) return false;
	if (!images.load("back", "back.png")) return false;
	if (!images.load("gameoverT", "gameoverT.png")) return false;
	if (!images.load("linksad", "linksad.png")) return false;
	if (!images.load("popupTienda", "popupTienda.png")) return false;
	if (!images.load("arbol1", "arbol1.png")) return false;
	if (!images.load("tienda", "tienda.png")) return false;
	if (!images.load("roca4", "roca4.png")) return false;
	if (!images.load("roca1", "roca1.png")) return false;
	if (!images.load("roca2", "roca2.png")) return false;
	if (!images.load("roca3", "roca3.png")) return false;
	if (!images.load("lore1", "lore1.png")) return false; 
	if (!images.load("lore2", "lore2.png")) return false;
	if (!images.load("lore3", "lore3.png")) return false;
	if (!images.load("lore4", "lore4.png")) return false;
	if (!images.load("lore5", "lore5.png")) return false;
	if (!images.load("lore6", "lore6.png")) return false;
	if (!images.load("lore7", "lore7.png")) return false;
	if (!images.load("lore8", "lore8.png")) return false;
	if (!images.load("lore9", "lore9.png")) return false;
	if (!images.load("lore10", "lore10.png")) return false;
	if (!images.load("lore11", "lore11.png")) return false;
	if (!images.load("lore12", "lore12.png")) return false;
	if (!images.load("lore13", "lore13.png")) return false; 
	if (!images.load("n0", "0.png")) return false;
	if (!images.load("n1", "1.png")) return false;
	if (!images.load("n2", "2.png")) return false;
	if (!images.load("n3", "3.png")) return false;
	if (!images.load("n4", "4.png")) return false;
	if (!images.load("n5", "5.png")) return false;
	if (!images.load("n6", "6.png")) return false;
	if (!images.load("n7", "7.png")) return false;
	if (!images.load("n8", "8.png")) return false;
	if (!images.load("n9", "9.png")) return false;
	if (!images.load("arbol2", "arbol2.png")) return false;
	if (!images.load("arbol3", "arbol3.png")) return false;
	if (!images.load("arbol4", "arbol4.png")) return false;
	if (!images.load("pausaT", "pausaT.png")) return false;
	if (!images.load("gallinaAzul", "gallinaAzul.png")) return false;
	if (!images.load("gallinaGolden", "gallinaGolden.png")) return false;
	if (!images.load("gallinaMarron", "gallinaMarron.png")) return false;
	if (!images.load("gallinaBlanca", "gallinaBlanca.png")) return false;
	if (!images.load("gallinaOscura", "gallinaOscura.png")) return false;
	
	Mix_PlayMusic(tracks.get("Intro"), 1);
	return true;
}

Game::~Game(){
	images.clear();
	tracks.clear();
	sfxs.clear();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::input() {
	while (SDL_PollEvent(&event) > 0) {
		switch (event.type) {
			case SDL_QUIT:
				isOpen = false;
				break;
			case SDL_KEYDOWN:
				if (!event.key.repeat) {
					if ((
						event.key.keysym.sym == SDLK_RETURN ||
						event.key.keysym.sym == SDLK_SPACE
					) && escena == LORE) cambiaEscena(JOC);
					if ((
						event.key.keysym.sym == SDLK_RETURN ||
						event.key.keysym.sym == SDLK_SPACE
					) && escena == MENU) cambiaEscena(LORE);
					if ((
						event.key.keysym.sym == SDLK_RETURN ||
						event.key.keysym.sym == SDLK_SPACE
					) && escena == GAMEOVER) cambiaEscena(MENU);
					if ((
						event.key.keysym.sym == SDLK_RETURN ||
						event.key.keysym.sym == SDLK_SPACE
					) && escena == INICI) cambiaEscena(MENU);
					if (event.key.keysym.sym == SDLK_q && (escena == PAUSA || escena == TIENDA)) cambiaEscena(MENU);
					if (event.key.keysym.sym == SDLK_t && escena == MENU) cambiaEscena(TIENDA);
					if (event.key.keysym.sym == SDLK_h) player.damage();
					if (event.key.keysym.sym == SDLK_F1) god = !god;
					if (event.key.keysym.sym == SDLK_F2) hardMode = !hardMode;
					if (event.key.keysym.sym == SDLK_m) {
						muted = !muted;
						botonSonido.img = images.get(muted ? "soundOff" : "soundOn");
						if(!paused) muted ? Mix_PauseMusic() : Mix_ResumeMusic();
					}
					if (event.key.keysym.sym == SDLK_p) {
						if (escena == JOC || escena == PAUSA) {
							paused = !paused;
							cambiaEscena(paused ? PAUSA : JOC);
							botonPlay.img = images.get(paused ? "pause" : "play");
							if (!muted) paused ? Mix_PauseMusic() : Mix_ResumeMusic();
						}
					}
				}
				break;
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_ENTER) {
					paused = false;
					//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
				}
				if (event.window.event == SDL_WINDOWEVENT_LEAVE) paused = true;
				break;
			case SDL_MOUSEMOTION:
				break;
			case SDL_MOUSEBUTTONDOWN:
				break;
		}
	}
}

void Game::cambiaEscena(Escena nuevaEscena) {
	if (escena != LORE && escena != PAUSA) while (Mix_PlayingMusic()) Mix_HaltMusic();
	switch (escena) {
		case INICI:
			break;
		case MENU:
			init();
			break;
		case LORE:
			break;
		case JOC:
			break;
		case GAMEOVER:
			init();
			break;
		case GUANYAT:
			init();
			break;
		case TIENDA:
			init();
			break;
		case PAUSA:
			break;
		default:
			break;
	}
	//if(escena == PAUSA && nuevaEscena == MENU)
	escena = nuevaEscena;
	switch (nuevaEscena) {
		if (escena != LORE && escena != PAUSA) while (Mix_PlayingMusic()) Mix_HaltMusic();
		case INICI:
			break;
		case MENU:
			Mix_PlayMusic(tracks.get("Menu"), -1);
			break;
		case LORE:
			Mix_PlayMusic(tracks.get("Gameplay"), -1);
			if (++loreShown > 13) loreShown = 1;
			break;
		case JOC:
			paused = false; 
			if (hardMode) {
				horda.dstRect->y = WINDOW_H - horda.dstRect->h * 3;
			}
			break;
		case GAMEOVER:
			Mix_PlayMusic(tracks.get("Game Over"), 1);
			break;
		case GUANYAT:
			break;
		case TIENDA:
			Mix_PlayMusic(tracks.get("Tienda"), -1);
			break;
		case PAUSA:
			break;
		default:
			break;
	}
}

void Game::update() {
	std::cout << mouseX << " - " << mouseY << std::endl;
	if (keyboard[SDL_SCANCODE_ESCAPE]) isOpen = false;
	switch (escena) {
		case INICI:
			break;
		case MENU:
			break;
		case LORE:
			break;
		case JOC:
			if (player.vides == 0) cambiaEscena(GAMEOVER);
			if(!paused){
				if (keyboard[SDL_SCANCODE_W]) {
					player.direccion = 1;
					player.update(0, -1);
				}
				if (keyboard[SDL_SCANCODE_S]) {
					player.direccion = 3;
					player.update(0, 1);
				}
				if (keyboard[SDL_SCANCODE_A]) {
					player.direccion = 0;
					if (!player.checkCollision(paredHitboxLeft.dstRect)) player.update(-1, 0);
				}
				if (keyboard[SDL_SCANCODE_D]) {
					player.direccion = 2;
					if(!player.checkCollision(paredHitboxRight.dstRect)) player.update(1, 0);
				}
				if (player.checkCollision(horda.dstRect)) {
					player.damage();
					player.dstRect->y -= horda.dstRect->h + 10;
				}
				if(camera.srcRect->y > 0) camera.update();
				else {
					std::cout << "FI DEL NIVELL!!" << std::endl;
				}
			}
			break;
		case GAMEOVER:
			break;
		case GUANYAT:
			break;
		case TIENDA:
			break;
		case PAUSA:
			break;
	}
}

void Game::draw() {
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	int w, h; 
	std::string num, s;
	switch (escena) {
		case INICI:
			SDL_QueryTexture(images.get("studio"), NULL, NULL, &w, &h);
			SDL_RenderCopy(renderer, images.get("studio"), NULL, new SDL_Rect({(WINDOW_W / 2) - 220, 150, 440, 440}));
			SDL_QueryTexture(images.get("enter"), NULL, NULL, &w, &h);
			SDL_RenderCopy(renderer, images.get("enter"), NULL, new SDL_Rect({ WINDOW_W - 55 - w / 2, 550, w / 2, h * 7/10}));
			break;
		case MENU:
			camera.sY = 0;
			camera.draw();
			player.draw();
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
			SDL_RenderFillRect(renderer, new SDL_Rect({ 0, 0, WINDOW_W, WINDOW_H }));
			botonSonido.draw();
			botonShop.draw();
			SDL_QueryTexture(images.get("start"), NULL, NULL, &w, &h);
			SDL_RenderCopy(renderer, images.get("start"), NULL, new SDL_Rect({ WINDOW_W - 100 - w / 3, (WINDOW_H / 2) - (h * 4 / 10 ) / 2, w *1/3, h * 4 / 10 }));
			break;
		case LORE:
			camera.draw();
			player.draw();
			botonSonido.draw();
			for (int i = 0; i < 3; i++) player.corazones[i].draw();
			if (loreShown == 0) loreShown = 1;
			SDL_QueryTexture(images.get("lore" + std::to_string(loreShown)), NULL, NULL, &w, &h);
			SDL_RenderCopy(renderer, images.get("lore" + std::to_string(loreShown)), NULL, new SDL_Rect({(WINDOW_W / 2) - (w * 7 / 5) / 2, WINDOW_H - h - 50, w * 7 / 5, h * 12 / 10}));
			break;
		case JOC:
			camera.sY = 2;
			camera.draw();
			botonSonido.draw();
			botonPlay.draw();
			if ((SDL_GetTicks() / 16) % 20 == 0 && !paused) player.animateX();
			if (!paused) player.animateY();
			horda.draw();
			if (hardMode) {
				for (int i = 0; i < 2; i++) {
					SDL_RenderCopy(renderer, images.get("horda"), NULL, new SDL_Rect({horda.dstRect->x, WINDOW_H - horda.dstRect->h * i, horda.dstRect->w, horda.dstRect->h}));
				}
			}
			player.draw();
			for (int i = 0; i < 3; i++) player.corazones[i].draw();
			SDL_RenderCopy(renderer, images.get("rupia"), NULL, new SDL_Rect({ WINDOW_W - 60, 90, 40, 40 }));
			num = std::to_string(player.money);
			for (int i = num.length() - 1; i >= 0; i--) {
				s = "n";
				s.append(1, num[i]);
				SDL_RenderCopy(renderer, images.get(s), NULL, new SDL_Rect({WINDOW_W - 80 - ((int)num.length() - i) * 30, 88, 35, 40}));
			}
			paredHitboxLeft.draw();
			paredHitboxRight.draw();
			//dstRect->x = WINDOW_W - 60 - i * 50;

			break;
		case GAMEOVER:
			camera.sY = 0;
			camera.draw();
			player.draw();
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
			SDL_RenderFillRect(renderer, new SDL_Rect({ 0, 0, WINDOW_W, WINDOW_H }));
			SDL_QueryTexture(images.get("gameoverT"), NULL, NULL, &w, &h);
			SDL_RenderCopy(renderer, images.get("gameoverT"), NULL, new SDL_Rect({ WINDOW_W - 100 - w / 3, 100, w * 1 / 3, h * 4 / 10 }));
			SDL_RenderCopy(renderer, images.get("linksad"), NULL, new SDL_Rect({ (WINDOW_W / 2) - 200, (WINDOW_H / 2), 350, 350 }));
			break;
		case GUANYAT:
			break;
		case TIENDA:
			camera.sY = 0;
			camera.draw();
			botonBack.draw();
			player.draw();
			SDL_QueryTexture(images.get("popupTienda"), NULL, NULL, &w, &h);
			SDL_RenderCopy(renderer, images.get("popupTienda"), NULL, new SDL_Rect({ WINDOW_W / 5 , 100, w , h }));
		
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 32);
			SDL_RenderFillRect(renderer, new SDL_Rect({ 0, 0, WINDOW_W, WINDOW_H }));
			botonSonido.draw();
			break;
		case PAUSA:
			camera.draw();
			player.draw();
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
			SDL_RenderFillRect(renderer, new SDL_Rect({ 0, 0, WINDOW_W, WINDOW_H }));
			botonSonido.draw();
			botonPlay.draw();
			botonBack.draw();
			for (int i = 0; i < 3; i++) player.corazones[i].draw();
			if ((SDL_GetTicks() / 16) % 40 == 0) showPausaText = !showPausaText;
			if (showPausaText) {
				SDL_QueryTexture(images.get("pausaT"), NULL, NULL, &w, &h);
				SDL_RenderCopy(renderer, images.get("pausaT"), NULL, new SDL_Rect({ (WINDOW_W / 2) - 320 , WINDOW_H / 2 - h * 2 / 10, w * 1 / 3, h * 4 / 10 }));
			}
			break;
	}
	SDL_RenderPresent(renderer);
}

void Game::destroy() {
	obstaculos.erase(std::remove_if(obstaculos.begin(), obstaculos.end(), [](const Cuadrado& o) {
		bool temp = o.dstRect->x < -o.dstRect->w || o.dstRect->x > WINDOW_W + o.dstRect->w || o.dstRect->y > WINDOW_H + o.dstRect->h;
		return o.disposable || temp;
	}), obstaculos.end());
	rupias.erase(std::remove_if(rupias.begin(), rupias.end(), [](const Cuadrado& o) {
		bool temp = o.dstRect->x < -o.dstRect->w || o.dstRect->x > WINDOW_W + o.dstRect->w || o.dstRect->y > WINDOW_H + o.dstRect->h;
		return o.disposable || temp;
	}), rupias.end());
	gallinas.erase(std::remove_if(gallinas.begin(), gallinas.end(), [](const Cuadrado& o) {
		bool temp = o.dstRect->x < -o.dstRect->w || o.dstRect->x > WINDOW_W + o.dstRect->w || o.dstRect->y > WINDOW_H + o.dstRect->h;
		return o.disposable || temp;
	}), gallinas.end());
}

void Game::loop() {
	input();
	update();
	draw();
	SDL_Delay(1000 / 60);
}
