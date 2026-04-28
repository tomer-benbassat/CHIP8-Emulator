#include "platform.h"

Platform::Platform(const char* title, int windowWidth, int windowHeight, int textureWidth, int textureHeight) 
    : display(title, windowWidth, windowHeight, textureWidth, textureHeight), lastTime(std::chrono::high_resolution_clock::now()) {}

Platform::~Platform() {}

double Platform::getDeltaTime() {
    auto currentTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> dt = currentTime - lastTime;
    lastTime = currentTime;
    return dt.count();
}

void Platform::render(const std::array<uint8_t, 64 * 32>& chip8Display) {
    uint32_t frameBuffer[64 * 32];
    for (int i = 0; i < 64 * 32; ++i) {
        frameBuffer[i] = (chip8Display[i] ? 0xFFFFFFFF : 0x00000000);
    }
    display.update(frameBuffer, 64 * sizeof(uint32_t));
}

bool Platform::processInput(uint8_t* keys) {
    return display.processInput(keys);
}