export module instruction;
import <variant>;
import <cstdint>;

namespace Instruction {
    /*
    export std::variant<RTypeInstruction, ITypeInstruction> parse(uint32_t word) {
        const auto opcode = word & 0b0111_1111;
        if opcode == 0b0110011  {
            return reinterpret_cast<RTypeInstruction>(word);
        } else {
            throw runtime_error("Couldn't parse instruction");
        }

    }
    */

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
}
