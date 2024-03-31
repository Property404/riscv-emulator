export module memory;

import<map>;
import<vector>;
import<cstdint>;
import<typeinfo>;
import test_harness;


export class Memory {
    std::map<size_t, uint8_t> memory;

public:
    Memory(std::vector<uint8_t> vec) {
        for (size_t i=0; i < vec.size(); i++) {
            this->memory[i] = vec[i];
        }
    }

    Memory() {
    }

    uint8_t load8(size_t dest) const {
        return this->memory.at(dest);
    }

    void store8(size_t dest, uint8_t val) {
        this->memory[dest] = val;
    }

    uint32_t load32(size_t dest) const {
        return this->memory.at(dest + 3) << 24 |
        this->memory.at(dest + 2) << 16 |
        this->memory.at(dest + 1) << 8 |
            this-> memory.at(dest);
    }

    void store32(size_t dest, uint32_t val) {
        memory[dest + 0] = (val >> 0) & 0xFF;
        memory[dest + 1] = (val >> 8) & 0xFF;
        memory[dest + 2] = (val >> 16) & 0xFF;
        memory[dest + 3] = (val >> 24) & 0xFF;
    }
};

#include "test/harness.h"
TEST_CASE(basic, {
    Memory memory({0xba,0xdd,0xec,0xaf});
    REQUIRE( memory.load8(0) == 0xba );

    memory.store32(0, 0xDEADBEEF);
    REQUIRE( memory.load32(0) == 0xDEADBEEF );
});
