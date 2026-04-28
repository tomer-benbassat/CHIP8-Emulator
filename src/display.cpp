#include "display.h"
#include <iostream>



Display::Display(const char* title, int windowWidth, int windowHeight, int textureWidth, int textureHeight) {    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(title, 
                              SDL_WINDOWPOS_CENTERED, 
                              SDL_WINDOWPOS_CENTERED, 
                              windowWidth, 
                              windowHeight, 
                              SDL_WINDOW_SHOWN);

    // sdl renderer accelerated tell sdl to use the hardware accelerated renderer instead of the software renderer (important for embedded systems)
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    texture = SDL_CreateTexture(renderer, 
                                SDL_PIXELFORMAT_RGBA8888, 
                                SDL_TEXTUREACCESS_STREAMING, 
                                textureWidth, 
                                textureHeight);
}


Display::~Display() {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}



void Display::update(const void* buffer , int pitch) {
    SDL_UpdateTexture(texture, nullptr, buffer, pitch);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

bool Display::processInput(uint8_t* keys) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) return true;

        if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
            bool pressed = (event.type == SDL_KEYDOWN);
            switch (event.key.keysym.sym) {
                case SDLK_x: keys[0x0] = pressed; break;
                case SDLK_1: keys[0x1] = pressed; break;
                case SDLK_2: keys[0x2] = pressed; break;
                case SDLK_3: keys[0x3] = pressed; break;
                case SDLK_q: keys[0x4] = pressed; break;
                case SDLK_w: keys[0x5] = pressed; break;
                case SDLK_e: keys[0x6] = pressed; break;
                case SDLK_a: keys[0x7] = pressed; break;
                case SDLK_s: keys[0x8] = pressed; break;
                case SDLK_d: keys[0x9] = pressed; break;
                case SDLK_z: keys[0xA] = pressed; break;
                case SDLK_c: keys[0xB] = pressed; break;
                case SDLK_4: keys[0xC] = pressed; break;
                case SDLK_r: keys[0xD] = pressed; break;
                case SDLK_f: keys[0xE] = pressed; break;
                case SDLK_v: keys[0xF] = pressed; break;
            }
        }
    }
    return false;
}

void Display::clear() {
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}