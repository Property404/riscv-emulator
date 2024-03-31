export module instruction;
import <variant>;
import <cstdint>;
import <stdexcept>;
import test_harness;


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

         int32_t sext_imm() const {
             const uint32_t MASK = 0x800;
             const bool is_negative = this->imm & MASK;
             const uint32_t value  = this->imm & (MASK - 1);
             if (is_negative) {
                 return -value;
             } else {
                 return value;
             }
         }
    };

    export std::variant<RTypeInstruction, ITypeInstruction> parse(uint32_t word) {
        const auto opcode = word & 0b0111'1111;
        if (opcode == 0b0110011)  {
            return std::bit_cast<RTypeInstruction>(word);
        } else if(opcode == 0b0010011) {
            return std::bit_cast<ITypeInstruction>(word);
        } else {
            throw std::runtime_error("Couldn't parse instruction");
        }

    }
}

#include "test/harness.h"
TEST_CASE(parse_i_type, {
    using namespace Instruction;
    const auto raw_instr = Instruction::parse(0b100000001111'11100'000'00111'0010011);
    const auto instr = std::get<ITypeInstruction>(raw_instr);

    REQUIRE_EQUALS(instr.opcode, 0b0010011u);
    REQUIRE_EQUALS(instr.rd, 0b00111u);
    REQUIRE_EQUALS(instr.funct3, 0b000u);
    REQUIRE_EQUALS(instr.rs1, 0b11100u);
    REQUIRE_EQUALS(instr.imm, 0b100000001111u);
    REQUIRE_EQUALS(instr.sext_imm(), -0xF);
});
