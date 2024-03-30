export module memory;
import <map>;
import <vector>;

export class Memory {
    std::map<size_t, uint8_t> memory;

public:
    Memory(vector<u8> vec) {
        for (int i=0; i < vec.length(); i++) {
            this->memory[i] = vec[i];
        }
    }

    uint8_t load8(size_t dest) const {
        return this->memory.at(dest);
    }

    uint16_t load16(size_t dest) const {
        this->memory.at(dest + 1) << 8 |
            this-> memory.at(dest);
    }

    uint32_t load32(size_t dest) const {
        this->memory.at(dest + 3) << 24 |
        this->memory.at(dest + 2) << 16 |
        this->memory.at(dest + 1) << 8 |
            this-> memory.at(dest);
    }
}
