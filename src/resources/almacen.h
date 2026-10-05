#pragma once
#include "general.h"

template <typename T>
class Almacen {
	public:
		std::map<std::string, T> mapa;

		void clear();

		bool load(const std::string &, const std::string &);

		T get(std::string name) {
			auto entry = mapa.find(name);
			return entry == mapa.end() ? T{} : entry->second;
		}
};