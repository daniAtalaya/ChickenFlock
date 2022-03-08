#pragma comment( lib, "SDL/x64/SDL2.lib" )
#pragma comment( lib, "SDL/x64/SDL2main.lib" )
#pragma comment( lib, "SDL_image/x64/SDL2_image.lib" )
#pragma comment( lib, "SDL_Mixer/x64/SDL2_mixer.lib" )
#include "SDL_Mixer/include/SDL_mixer.h"
#include "SDL_image/include/SDL_image.h"
#include "SDL/include/SDL.h"
#include <string>
#include <vector>
#include <iostream>
#define WINDOW_W 800
#define WINDOW_H 600
#define WINDOW_T "Daniel Atalaya's Seventh SDL2 Window"
#define INIT_R srand(time(NULL))
#define R_NUM(min, max) min + rand() % ((max + 1) - min)