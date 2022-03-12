#pragma once
#include "general.h"
class Sprite
{
    public:
        Sprite(char const* path, int row, int column);

        void selectSprite(int x, int y);
        void draw(SDL_Surface* window_surface, SDL_Rect* position);

    private:
        SDL_Rect     m_clip;
        SDL_Surface* m_spritesheet_image;
};