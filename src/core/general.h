#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>
#include <vector>
#include <cmath>
#include <map>
#include <iostream>
#include <functional>

#define WINDOW_W 960
#define WINDOW_H 900
#define INIT_R srand(time(NULL))
#define R_NUM(min, max) min + rand() % ((max + 1) - min)