export module emulator;

import <cstdint>;
import <cstddef>;

import memory;

export class Emulator {
    uint32_t registers[32];
    Memory memory;
    size_t ip = 0;

    public:
    Emulator(size_t ip, Memory memory);

    void step() {
        const auto next_instruction = memory.load32(this->ip);
    }
};
