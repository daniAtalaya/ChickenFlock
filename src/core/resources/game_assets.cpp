#include "game_assets.h"

bool GameAssets::load(SDL_Renderer* renderer, MIX_Mixer* mixer) {
	//sfxs
	if (!sfxs.load("dañoGallina", "DanoContraGallina.wav", nullptr, mixer)) return false;
	if (!sfxs.load("dañoQueja", "Danoqueja.wav", nullptr, mixer)) return false;
	if (!sfxs.load("muerteGallina", "muertegallinaex.wav", nullptr, mixer)) return false;
	if (!sfxs.load("disparoFlecha", "disparoflecha.wav", nullptr, mixer)) return false;
	if (!sfxs.load("SMoneda", "Sonidomoneda.wav", nullptr, mixer)) return false;
	if (!sfxs.load("MultitudG", "multitudG.wav", nullptr, mixer)) return false;
	if (!sfxs.load("SStart", "sonidostart.wav", nullptr, mixer)) return false;
	if (!tracks.load("Creditos", "Creditos.ogg", nullptr, mixer, true)) return false;
	if (!tracks.load("Victoria", "VICTORIA.ogg", nullptr, mixer, true)) return false;
	if (!tracks.load("Intro", "Intro Colibri Studios.ogg", nullptr, mixer, true)) return false;
	if (!tracks.load("Game Over", "Game Over.ogg", nullptr, mixer, true)) return false;
	if (!tracks.load("Gameplay", "Gameplay.ogg", nullptr, mixer, true)) return false;
	if (!tracks.load("Menu", "Menu.ogg", nullptr, mixer, true)) return false;
	if (!tracks.load("Tienda", "Tienda.ogg", nullptr, mixer, true)) return false;
	if (!tracks.load("sonido de start", "sonidostart.ogg", nullptr, mixer, true)) return false;
	//if (!tracks.load("Multidud de gallinas", "Multidud de gallinas.ogg")) return false;
	if (!images.load("soundOn", "soundOn.png", renderer)) return false;
	if (!images.load("soundOff", "soundOff.png", renderer)) return false;
	if (!images.load("mapa3", "mapa3.png", renderer)) return false;
	if (!images.load("play", "play.png", renderer)) return false;
	if (!images.load("continuara", "continuara.png", renderer)) return false;
	if (!images.load("soldOut", "soldOut.png", renderer)) return false;
	if (!images.load("creditos", "creditos.png", renderer)) return false;
	if (!images.load("link", "link.png", renderer)) return false;
	if (!images.load("rupia1", "rupia1.png", renderer)) return false;
	if (!images.load("rupia2", "rupia2.png", renderer)) return false;
	if (!images.load("rupia3", "rupia3.png", renderer)) return false;
	if (!images.load("rupia4", "rupia4.png", renderer)) return false;
	if (!images.load("pajaro", "pajaro.png", renderer)) return false;
	if (!images.load("mascota", "mascota.png", renderer)) return false;
	if (!images.load("corazon", "corazon.png", renderer)) return false;
	if (!images.load("corazont", "corazont.png", renderer)) return false;
	if (!images.load("enter", "press_enter.png", renderer)) return false;
	if (!images.load("pause", "pause.png", renderer)) return false;
	if (!images.load("studio", "studio.png", renderer)) return false;
	if (!images.load("horda", "horda.png", renderer)) return false;
	if (!images.load("flecha", "flecha.png", renderer)) return false;
	if (!images.load("flechab", "flecha_b.png", renderer)) return false;
	if (!images.load("start", "start.png", renderer)) return false;
	if (!images.load("back", "back.png", renderer)) return false;
	if (!images.load("gameoverT", "gameoverT.png", renderer)) return false;
	if (!images.load("linksad", "linksad.png", renderer)) return false;
	if (!images.load("popupTienda", "popupTienda.png", renderer)) return false;
	if (!images.load("arbol1", "arbol1.png", renderer)) return false;
	if (!images.load("tienda", "tienda.png", renderer)) return false;
	if (!images.load("roca4", "roca4.png", renderer)) return false;
	if (!images.load("roca1", "roca1.png", renderer)) return false;
	if (!images.load("roca2", "roca2.png", renderer)) return false;
	if (!images.load("roca3", "roca3.png", renderer)) return false;
	if (!images.load("lore1", "lore2.png", renderer)) return false;
	if (!images.load("tiendalore1", "Tiendalore_9.png", renderer)) return false;
	if (!images.load("arbol2", "arbol2.png", renderer)) return false;
	if (!images.load("arbol3", "arbol3.png", renderer)) return false;
	if (!images.load("arbol4", "arbol4.png", renderer)) return false;
	if (!images.load("pausaT", "pausaT.png", renderer)) return false;
	if (!images.load("gallina1", "gallinaBlanca.png", renderer)) return false;
	if (!images.load("gallina2", "gallinaMarron.png", renderer)) return false;
	if (!images.load("gallina3", "gallinaAzul.png", renderer)) return false;
	if (!images.load("gallina4", "gallinaOscura.png", renderer)) return false;
	if (!images.load("gallina5", "gallinaGolden.png", renderer)) return false;
	if (!images.load("hardcore", "hardcore.png", renderer)) return false;
	if (!images.load("winner", "winner.png", renderer)) return false;
	if (!images.load("tituloCockFlock", "tituloCockFlock.png", renderer)) return false;
	if (!images.load("creditosBoton", "creditosBoton.png", renderer)) return false;
	if (!images.load("avestruz", "avestruz.png", renderer)) return false;
	return true;
}


void GameAssets::clear() {
	if (cleared) {
		return;
	}
	images.clear();
	tracks.clear();
	sfxs.clear();
	cleared = true;
}
