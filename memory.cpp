export module memory;

import<map>;
import<vector>;
import<iostream>;
import<stdexcept>;
import<sstream>;
import<cstdint>;
import test_harness;

export class Memory {
    std::map<size_t, uint8_t> memory;

public:
    Memory(std::vector<uint8_t> vec) {
        for (size_t i=0; i < vec.size(); i++) {
            this->memory[i] = vec[i];
        }
    }

    Memory(size_t load_address, const std::vector<uint8_t>& vec) {
        for (size_t i=0; i < vec.size(); i++) {
            this->memory[i+load_address] = vec[i];
        }
    }

    Memory() {
    }

    uint8_t load8(size_t dest) const {
        if (!memory.contains(dest)) {
            std::stringstream ss;
            ss << "Unable to access byte at 0x" << std::hex << dest;
            throw std::runtime_error(ss.str());
        }
        return this->memory.at(dest);
    }

    void store8(size_t dest, uint8_t val) {
        this->memory[dest] = val;
    }

    uint32_t load32(size_t dest) const {
        if (!memory.contains(dest)) {
            std::stringstream ss;
            ss << "Unable to access 32-bit word at 0x" << std::hex << dest;
            throw std::runtime_error(ss.str());
        }
        return this->memory.at(dest + 3) << 24 |
        this->memory.at(dest + 2) << 16 |
        this->memory.at(dest + 1) << 8 |
            this-> memory.at(dest);
    }

    uint64_t load64(size_t dest) const {
        if (!memory.contains(dest)) {
            std::stringstream ss;
            ss << "Unable to access double at 0x" << std::hex << dest;
            throw std::runtime_error(ss.str());
        }
        return
        (uint64_t)(this->memory.at(dest + 7)) << 56 |
        (uint64_t)(this->memory.at(dest + 6)) << 48 |
        (uint64_t)(this->memory.at(dest + 5)) << 40 |
        (uint64_t)(this->memory.at(dest + 4)) << 32 |
        this->memory.at(dest + 3) << 24 |
        this->memory.at(dest + 2) << 16 |
        this->memory.at(dest + 1) << 8 |
            this-> memory.at(dest);
    }

    void store32(size_t dest, uint32_t val) {
        //std::cout << "Storing at " << std::hex << dest <<std::endl;
        memory[dest + 0] = (val >> 0) & 0xFF;
        memory[dest + 1] = (val >> 8) & 0xFF;
        memory[dest + 2] = (val >> 16) & 0xFF;
        memory[dest + 3] = (val >> 24) & 0xFF;
    }

    void store64(size_t dest, uint64_t val) {
        //std::cout << "Storing at " << std::hex << dest <<std::endl;
        memory[dest + 0] = (val >> 0) & 0xFF;
        memory[dest + 1] = (val >> 8) & 0xFF;
        memory[dest + 2] = (val >> 16) & 0xFF;
        memory[dest + 3] = (val >> 24) & 0xFF;
        memory[dest + 4] = (val >> 32) & 0xFF;
        memory[dest + 5] = (val >> 40) & 0xFF;
        memory[dest + 6] = (val >> 48) & 0xFF;
        memory[dest + 7] = (val >> 56) & 0xFF;
    }
};

#include "test/harness.h"
TEST_CASE(basic, {
    Memory memory({0xba,0xdd,0xec,0xaf});
    REQUIRE( memory.load8(0) == 0xba );

    memory.store32(0, 0xDEADBEEF);
    REQUIRE( memory.load32(0) == 0xDEADBEEF );
});
