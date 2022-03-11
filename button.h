#pragma once
#include "general.h"
class Button
{
	public:
		Button() {};
		void setHandler(std::function<void(void)> func) { function = func; };
		void action() { function(); };
	private:
		std::function<void(void)> function;
};

