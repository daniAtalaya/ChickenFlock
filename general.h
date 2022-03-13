#pragma comment( lib, "SDL/x64/SDL2.lib" )
#pragma comment( lib, "SDL/x64/SDL2main.lib" )
#pragma comment( lib, "SDL_image/x64/SDL2_image.lib" )
#pragma comment( lib, "SDL_Mixer/x64/SDL2_mixer.lib" )
#include "SDL_Mixer/include/SDL_mixer.h"
#include "SDL_image/include/SDL_image.h"
#include "SDL/include/SDL.h"
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
