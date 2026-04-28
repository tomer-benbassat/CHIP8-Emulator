#ifndef CHIP8_H
#define CHIP8_H

#include <array>
#include <cstdint>
#include <string>


class Chip8 {
public:
    Chip8();

    // Load ROM into memory
    void loadROM(const std::string& filename); 

    // Emulate one CPU cycle
    void fetch();
    void cycle();
    void decodeAndExecute();
    void updateTimers();

    // Input handling
    void setKey(uint8_t key, bool pressed);

    // Get display buffer (64x32)
    const std::array<uint8_t, 64 * 32>& getDisplay() const;

    // Flag to know when to redraw
    bool shouldDraw() const;
    void resetDrawFlag();

    // Get pointer to keypad for display input handling
    uint8_t* getKeypadPointer() { return keypad.data(); }

private: 

    // Opcode components to be filled after fetching
    uint8_t x = 0; // X register
    uint8_t y = 0; // Y register
    uint8_t kk = 0; // 8-bit immediate value
    uint8_t n = 0; // 4-bit immediate value
    uint16_t nnn = 0; // 12-bit immediate value


    // Memory (4KB)
    std::array<uint8_t, 4096> memory;

    // Registers V0–VF
    std::array<uint8_t, 16> V;

    // Index register
    uint16_t I;

    // Program counter
    uint16_t pc;

    // Stack (16 levels)
    std::array<uint16_t, 16> stack;
    uint8_t sp;

    // Timers
    uint8_t delay_timer;
    uint8_t sound_timer;

    // Display (64x32)
    std::array<uint8_t, 64 * 32> display;

    // Keypad (16 keys)
    std::array<uint8_t, 16> keypad;

    // Current opcode
    uint16_t opcode;

    // Draw flag
    bool draw_flag;

    // Opcodes (declare, you implement)
    void OP_00E0(); // CLS - Clear the display
    void OP_00EE(); // RET - Return from subroutine
    void OP_1NNN(); // JMP - Jump to address NNN
    void OP_2NNN(); // CALL - Call subroutine at address NNN
    void OP_3XKK(); // SE - Skip if V[X] == KK
    void OP_4XKK(); // SNE - Skip if V[X] != KK
    void OP_5XY0(); // SE - Skip if V[X] == V[Y]
    void OP_6XKK(); // LD - Load V[X] = KK
    void OP_7XKK(); // ADD - Add KK to V[X]
    void OP_8XY0(); // LD - Load V[X] = V[Y]
    void OP_8XY1(); // OR - Bitwise OR V[X] = V[X] | V[Y]
    void OP_8XY2(); // AND - Bitwise AND V[X] = V[X] & V[Y]
    void OP_8XY3(); // XOR - Bitwise XOR V[X] = V[X] ^ V[Y]
    void OP_8XY4(); // ADD - Add V[Y] to V[X]
    void OP_8XY5(); // SUB - Subtract V[Y] from V[X]
    void OP_8XY6(); // SHR - Right shift V[X]
    void OP_8XY7(); // SUBN - Subtract V[X] from V[Y]
    void OP_8XYE(); // SHL - Left shift V[X]
    void OP_9XY0(); // SNE - Skip if V[X] != V[Y]
    void OP_ANNN(); // LD - Load I = NNN
    void OP_BNNN(); // JP V0, addr
    void OP_CXKK(); // RND Vx, byte
    void OP_DXYN(); // DRW - Draw sprite at (V[X], V[Y]) with height N
    void OP_EX9E(); // SKP - Skip if key V[X] is pressed
    void OP_EXA1(); // SKNP - Skip if key V[X] is not pressed
    void OP_FX07(); // LD - Load V[X] = delay_timer
    void OP_FX0A(); // LD Vx, K
    void OP_FX15(); // LD - Load delay_timer = V[X]
    void OP_FX18(); // LD - Load sound_timer = V[X]
    void OP_FX1E(); // ADD - Add V[X] to I
    void OP_FX29(); // LD - Load I = sprite_address
    void OP_FX33(); // LD - Store V[X] in I as BCD
    void OP_FX55(); // LD - Store V[0..X] in memory starting at I
    void OP_FX65(); // LD - Load V[0..X] from memory starting at I

};

#endif