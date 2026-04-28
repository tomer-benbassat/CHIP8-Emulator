#ifndef PLATFORM_H
#define PLATFORM_H

#include <chrono>
#include <cstdint>
#include "display.h"

class Platform {
public:
    Platform(const char* title, int windowWidth, int windowHeight, int textureWidth, int textureHeight);
    ~Platform();
    double getDeltaTime();
    void render(const std::array<uint8_t, 64 * 32>& chip8Display);
    bool processInput(uint8_t* keys);
private:
    Display display;
    std::chrono::high_resolution_clock::time_point lastTime;
};
#endif
