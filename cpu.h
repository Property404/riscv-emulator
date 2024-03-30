export module emulator;
import <map>;
import <vector>;

export class Emulator {
    uint32_t registers[32];
    std::map<size_t, uint8_t> memory;
    size_t ip;

    void load32(dest) const {
        this->memory.at(dest)
    }

    public:
    Emulator(size_t ip, vector<uint8_t> memory);

    void step() {
        parse_instruction()
    }
}
