#include "chip8.h"
#include "platform.h"
#include <thread>
#include <chrono>


int main() {
    Chip8 chip8;
    Platform platform("CHIP-8 Emulator", 640, 320, 64, 32);
    
    chip8.loadROM("test_roms/IBM Logo.ch8");

    double accumulator_cpu = 0;
    double accumulator_timer = 0;

    bool quit = false;
    while (!quit) {
        double dt = platform.getDeltaTime();
        accumulator_cpu += dt;
        accumulator_timer += dt;

        quit = platform.processInput(chip8.getKeypadPointer());

        while (accumulator_cpu >= 2.0) {
            chip8.cycle();
            accumulator_cpu -= 2.0;
        }
        if (accumulator_timer >= 16.67) {
            chip8.updateTimers();
            accumulator_timer -= 16.67;
        }

        if (chip8.shouldDraw()) {
            platform.render(chip8.getDisplay());
            chip8.resetDrawFlag();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return 0;
}