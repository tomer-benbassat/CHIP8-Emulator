#include "chip8.h"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>


// CHIP-8 fontset
static const uint8_t chip8_fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

// Starting address of the CHIP-8 program
const unsigned int START_ADDRESS = 0x200;



// Constructor
Chip8::Chip8() {
    memory.fill(0);
    V.fill(0);
    I = 0;
    pc = START_ADDRESS;
    stack.fill(0);
    sp = 0;
    delay_timer = 0;
    sound_timer = 0;
    display.fill(0);
    keypad.fill(0);
    opcode = 0;
    draw_flag = false;

    for (unsigned int i=0 ; i<80 ; i++){
        memory[i] = chip8_fontset[i];
    }
}

// Load ROM into memory
void Chip8::loadROM(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::streamsize size = file.tellg();
    if (size <= 0) {
        std::cerr << "Error determining file size\n";
        return;
    }  
    file.seekg(0, std::ios::beg);

    if (size > (4096 - START_ADDRESS)) {
        std::cerr << "ROM too large!" << std::endl;
        return;
    }

    file.read(reinterpret_cast<char*>(memory.data() + START_ADDRESS), size);
    file.close();
}

// Emulate one CPU cycle
void Chip8::cycle() {
    fetch();
    decodeAndExecute();
    updateTimers();
}



// Fetch opcode
void Chip8::fetch() {
    opcode = (memory[pc] << 8u) | memory[pc + 1];
    pc += 2;

    x = (opcode >> 8) & 0x0F;
    y = (opcode >> 4) & 0x0F;
    kk = opcode & 0x00FF;
    n = opcode & 0x000F;
    nnn = opcode & 0x0FFF;
}

// Decode and execute opcode
void Chip8::decodeAndExecute() {
    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode & 0x00FF) {
                case 0x00E0:
                    OP_00E0();
                    break;
                case 0x00EE:
                    OP_00EE();
                    break;
                default:
                    std::cerr << "Unknown opcode: " << std::hex << opcode << std::endl;
                    break;
            }
            break;

        case 0x1000:
            OP_1NNN();
            break;
        
        case 0x2000:
            OP_2NNN();
            break;

        case 0x3000:
            OP_3XKK();
            break;

        case 0x4000:
            OP_4XKK();
            break;

        case 0x5000:
            if ((opcode & 0x000F) == 0x0)
                OP_5XY0();
            else
                std::cerr << "Unknown opcode: " << std::hex << opcode << std::endl;
            break;

        case 0x6000:
            OP_6XKK();
            break;

        case 0x7000:
            OP_7XKK();
            break;
        
        case 0x8000:
            switch (opcode & 0x000F) {
                case 0x0:
                    OP_8XY0();
                    break;
                case 0x1:
                    OP_8XY1();
                    break;
                case 0x2:
                    OP_8XY2();
                    break;
                case 0x3:
                    OP_8XY3();
                    break;
                case 0x4:
                    OP_8XY4();
                    break;
                case 0x5:
                    OP_8XY5();
                    break;
                case 0x6:
                    OP_8XY6();
                    break;
                case 0x7:
                    OP_8XY7();
                    break;
                case 0xE:
                    OP_8XYE();
                    break;
                default:
                    std::cerr << "Unknown opcode: " << std::hex << opcode << std::endl;
                    break;
            }
            break;
        
        case 0x9000:
            if ((opcode & 0x000F) == 0x0)
                OP_9XY0();
            else
                std::cerr << "Unknown opcode: " << std::hex << opcode << std::endl;
            break;

        case 0xA000:
            OP_ANNN();
            break;

        case 0xB000:
            OP_BNNN();
            break;

        case 0xC000:
            OP_CXKK();
            break;

        case 0xD000:
            OP_DXYN();
            break;

        case 0xE000:
            switch (opcode & 0x00FF) {
                case 0x9E:
                    OP_EX9E();
                    break;
                case 0xA1:
                    OP_EXA1();
                    break;
                default:
                    std::cerr << "Unknown opcode: " << std::hex << opcode << std::endl;
                    break;
            }
            break;
        
        case 0xF000:
            switch (opcode & 0x00FF) {
                case 0x07:
                    OP_FX07();
                    break;
                case 0x0A:
                    OP_FX0A();
                    break;
                case 0x15:
                    OP_FX15();
                    break;
                case 0x18:
                    OP_FX18();
                    break;
                case 0x1E:
                    OP_FX1E();
                    break;
                case 0x29:
                    OP_FX29();
                    break;
                case 0x33:
                    OP_FX33();
                    break;
                case 0x55:
                    OP_FX55();
                    break;
                case 0x65:
                    OP_FX65();
                    break;
            }
            break;

        default:
            std::cerr << "Unknown opcode: " << std::hex << opcode << std::endl;
            break;
    }
}

// Update timers
void Chip8::updateTimers() {
    if (delay_timer > 0)
        delay_timer--;
    if (sound_timer > 0)
        sound_timer--;
}

// Get display buffer
const std::array<uint8_t, 64 * 32>& Chip8::getDisplay() const {
    return display;
}

// Check if display needs to be redrawn
bool Chip8::shouldDraw() const {
    return draw_flag;
}

// Reset draw flag
void Chip8::resetDrawFlag() {
    draw_flag = false;
}

// Set key state
void Chip8::setKey(uint8_t key, bool pressed) {
    if (key < 16) {
        keypad[key] = pressed;
    }
}
