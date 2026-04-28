#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include <cstdint>
#include <iostream>
#include "chip8.h"


class Display{
public:
    //Display(const void* buffer , int pitch);
    Display(const char* title, int windowWidth, int windowHeight, int textureWidth, int textureHeight);
    ~Display();

    void update(const void* buffer , int pitch);
    bool processInput(uint8_t* keys); 
    void clear();
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
};
#endif