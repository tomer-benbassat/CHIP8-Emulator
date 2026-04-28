#include "chip8.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

void Chip8::OP_00E0() {
    display.fill(0);
    draw_flag = true;
}

void Chip8::OP_00EE(){
    sp--;
    pc = stack[sp];
}

void Chip8::OP_1NNN() {
    pc = nnn;
}

void Chip8::OP_2NNN() {
    stack[sp] = pc;
    sp++;
    pc = nnn;
}

void Chip8::OP_3XKK() {
    if (V[x] == kk)
        pc += 2;
}

void Chip8::OP_4XKK() {
    if (V[x] != kk)
        pc += 2;
}

void Chip8::OP_5XY0() {
    if (V[x] == V[y])
        pc += 2;
}

void Chip8::OP_6XKK() {
    V[x] = kk;
}

void Chip8::OP_7XKK() {
    V[x] += kk;
}

void Chip8::OP_8XY0() {
    V[x] = V[y];
}

void Chip8::OP_8XY1() {
    V[x] |= V[y];
}

void Chip8::OP_8XY2() {
    V[x] &= V[y];
}

void Chip8::OP_8XY3() {
    V[x] ^= V[y];
}

void Chip8::OP_8XY4() {
    V[x] += V[y];
}

void Chip8::OP_8XY5() {
    V[x] -= V[y];
}

void Chip8::OP_8XY6() {
    V[x] >>= 1;
}

void Chip8::OP_8XY7() {
    V[x] = V[y] - V[x];
}

void Chip8::OP_8XYE() {
    V[x] <<= 1;
}

void Chip8::OP_9XY0() {
    if (V[x] != V[y])
        pc += 2;
}

void Chip8::OP_ANNN() {
    I = nnn;
}

void Chip8::OP_BNNN() {
    pc = V[0] + nnn;
}

void Chip8::OP_CXKK() {
    V[x] = rand() & kk;
}

void Chip8::OP_DXYN() {
    // Draw sprite at (V[X], V[Y]) with height N
    uint8_t x_pos = V[x];
    uint8_t y_pos = V[y];
    uint8_t height = n;
    
    // Clear collision flag
    V[0xF] = 0;
    
    // Draw each row of the sprite
    for (uint8_t row = 0; row < height; ++row) {
        uint8_t sprite_byte = memory[I + row];
        uint8_t y_coord = (y_pos + row) % 32;
        
        // Draw each pixel in the row
        for (uint8_t bit = 0; bit < 8; ++bit) {
            uint8_t x_coord = (x_pos + bit) % 64;
            
            // Check if the pixel is set in the sprite
            if ((sprite_byte >> (7 - bit)) & 0x1) {
                uint32_t pixel_index = y_coord * 64 + x_coord;
                
                // Check for collision (drawing on an already set pixel)
                if (display[pixel_index] == 1) {
                    V[0xF] = 1;
                }
                
                // Flip the pixel (XOR)
                display[pixel_index] ^= 1;
            }
        }
    }
    
    draw_flag = true;
}

void Chip8::OP_EX9E() {
    if (keypad[V[x]])
        pc += 2;
}

void Chip8::OP_EXA1() {
    if (!keypad[V[x]])
        pc += 2;
}

void Chip8::OP_FX07() {
    V[x] = delay_timer;
}

void Chip8::OP_FX0A() {
    // Wait for key press
    bool key_pressed = false;
    for (int i = 0; i < 16; ++i) {
        if (keypad[i]) {
            V[x] = i;
            key_pressed = true;
            break;
        }
    }
    
    if (!key_pressed) {
        pc -= 2;  // Re-execute this instruction
    }
}

void Chip8::OP_FX15() {
    delay_timer = V[x];
}

void Chip8::OP_FX18() {
    sound_timer = V[x];
}

void Chip8::OP_FX1E() {
    I += V[x];
}

void Chip8::OP_FX29() {
    I = V[x] * 5;
}

void Chip8::OP_FX33() {
    memory[I] = V[x] / 100;
    memory[I + 1] = (V[x] / 10) % 10;
    memory[I + 2] = V[x] % 10;
}

void Chip8::OP_FX55() {
    for (int i = 0; i <= x; ++i) {
        memory[I + i] = V[i];
    }
}

void Chip8::OP_FX65() {
    for (int i = 0; i <= x; ++i) {
        V[i] = memory[I + i];
    }
}   