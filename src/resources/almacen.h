#pragma once
#include "general.h"
template <typename T>
class Almacen {
	public:
		void clear();
		std::map<std::string, T> mapa;
		bool load(std::string, std::string);
		T get(std::string name) { return mapa[name]; }
};