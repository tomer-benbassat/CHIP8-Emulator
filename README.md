# CHIP-8 Emulator (C++17 & SDL2)

A high-performance, modular **CHIP-8 interpreter** written in **C++17**, featuring a custom hardware abstraction layer and cycle-accurate timing control. This project demonstrates low-level systems programming, instruction decoding, and real-time synchronization.

## 🚀 Key Features

* **Modular Architecture**: Separated into distinct logic, platform, and display layers to ensure high portability and maintainability.
* **Hardware Abstraction Layer (HAL)**: Isolated SDL2 dependencies from the core emulator logic, simulating professional embedded software design.
* **Precise Timing**: Implemented an **Accumulator-based game loop** to decouple the 500Hz CPU cycle from the 60Hz delay and sound timers.
* **Cross-Platform Graphics**: Powered by **SDL2** using hardware-accelerated rendering and streaming textures for efficient frame buffer updates.
* **Robust I/O**: Full mapping of the original 16-key hex keypad to modern QWERTY layouts.



## 🛠️ Built With

* **C++17**: Utilizing modern STL containers (`std::array`, `std::chrono`) for memory safety and performance.
* **SDL2**: For cross-platform window management, rendering, and event handling.
* **Makefile**: Custom build system for automated compilation and linking.

## 📂 Project Structure

* `include/`: Header files defining the system interfaces 
    (`chip8.h`, `platform.h`, `display.h`).
* `src/`: Core implementation files, including the instruction decoder and opcode logic.
* `test_roms/`: Directory for loading and testing original CHIP-8 ROM files.

## ⚙️ Installation & Usage

1. **Dependencies**: Ensure you have `SDL2` installed on your system.
   ```bash
   sudo apt install libsdl2-dev