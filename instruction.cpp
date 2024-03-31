export module instruction;
import <variant>;
import <cstdint>;
import <stdexcept>;


namespace Instruction {
    export struct RTypeInstruction {
         uint32_t opcode: 7;
         uint32_t rd: 5;
         uint32_t funct3: 3;
         uint32_t rs1: 5;
         uint32_t rs2: 5;
         uint32_t funct7: 7;
    };

    export struct ITypeInstruction {
         uint32_t opcode: 7;
         uint32_t rd: 5;
         uint32_t funct3: 3;
         uint32_t rs1: 5;
         uint32_t imm: 12;
    };

    export std::variant<RTypeInstruction, ITypeInstruction> parse(uint32_t word) {
        const auto opcode = word & 0b0111'1111;
        if (opcode == 0b0110011)  {
            return std::bit_cast<RTypeInstruction>(word);
        } else {
            throw std::runtime_error("Couldn't parse instruction");
        }

    }
}
