#pragma once
#include "general.h"
template <typename T>
class Almacen {
	public:
		std::map<std::string, T> mapa;
		bool loadImage(std::string, std::string);
		bool loadMusic(std::string, std::string);
		bool loadEffect(std::string, std::string);
		T get(std::string name) { return mapa[name]; }
};