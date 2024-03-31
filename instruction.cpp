export module instruction;
import <variant>;
import <string>;
import <cstdint>;
import <stdexcept>;
import test_harness;

namespace Instruction {
    export struct STypeInstruction {
         uint32_t opcode: 7;
         uint32_t imm5: 5;
         uint32_t funct3: 3;
         uint32_t rs1: 5;
         uint32_t rs2: 5;
         uint32_t imm7: 7;

         int64_t sext_imm() const {
             const uint64_t MASK = 0x800;
             const uint64_t imm = (imm7 << 5) + imm5;
             const bool is_negative = imm & MASK;
             const uint64_t value  = imm & (MASK - 1);
             if (is_negative) {
                 return value | ~(MASK-1);
             } else {
                 return value;
             }
         }
    };

    export struct UTypeInstruction {
         uint32_t opcode: 7;
         uint32_t rd: 5;
         uint32_t imm20: 20;

         uint32_t zext_imm() const {
             return this->imm20 << 12;
         }
    };

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

         int64_t sext_imm() const {
             const uint64_t MASK = 0x800;
             const bool is_negative = this->imm & MASK;
             const uint64_t value  = this->imm & (MASK - 1);
             if (is_negative) {
                 return value | ~(MASK-1);
             } else {
                 return value;
             }
         }

         uint64_t zext_imm() const {
             return this->imm;
         }
    };

    export struct BTypeInstruction {
         uint32_t opcode: 7;
         uint32_t imm11: 1;
         uint32_t imm4_1: 4;
         uint32_t funct3: 3;
         uint32_t rs1: 5;
         uint32_t rs2: 5;
         uint32_t imm10_5: 6;
         uint32_t imm12: 1;

         int64_t sext_imm() const {
             const bool is_negative = this->imm12;
             const uint64_t value  = this->zext_imm();
             if (is_negative) {
                 return value | ~((1<<12) - 1);
             } else {
                 return value;
             }
         }

         uint64_t zext_imm() const {
             return (this->imm4_1 << 1) | (this->imm10_5 << 5) | (this->imm11 << 11);
         }
    };

    export struct JTypeInstruction {
         uint32_t opcode: 7;
         uint32_t rd: 5;
         uint32_t imm19_12: 8;
         uint32_t imm11: 1;
         uint32_t imm10_1: 10;
         uint32_t imm20: 1;

         int64_t sext_imm() const {
             const bool is_negative = this->imm20;
             const uint64_t value  = this->zext_imm();
             if (is_negative) {
                 return value | ~((1<<20) - 1);
             } else {
                 return value;
             }
         }

         uint64_t zext_imm() const {
             return (this->imm19_12 << 12) | (this->imm11 << 11) | (this->imm10_1 << 1) | (this->imm20 << 20);
         }
    };

    export std::variant<JTypeInstruction,BTypeInstruction, RTypeInstruction, ITypeInstruction, STypeInstruction, UTypeInstruction> parse(uint32_t word) {
        const auto opcode = word & 0b0111'1111;
        if (opcode == 0b0110011)  {
            return std::bit_cast<RTypeInstruction>(word);
        } else if(opcode == 0b0010011 || opcode == 0b0000011 || opcode == 0b1110011 || opcode == 0b0011011 || opcode == 0b1100111) {
            return std::bit_cast<ITypeInstruction>(word);
        } else if(opcode == 0b0100011) {
            return std::bit_cast<STypeInstruction>(word);
        } else if(opcode == 0b0110111 || opcode == 0b0010111) {
            return std::bit_cast<UTypeInstruction>(word);
        } else if(opcode == 0b1100011) {
            return std::bit_cast<BTypeInstruction>(word);
        } else if(opcode == 0b1101111) {
            return std::bit_cast<JTypeInstruction>(word);
        } else {
            throw std::runtime_error(std::string{"Couldn't parse instructions with opcode: "} + std::to_string(opcode));
        }
    }
}


#include "test/harness.h"
TEST_CASE(parse_i_type, {
    using namespace Instruction;
    const auto raw_instr = Instruction::parse(0b111111111111'11100'000'00111'0010011);
    const auto instr = std::get<ITypeInstruction>(raw_instr);

    REQUIRE_EQUALS(instr.opcode, 0b0010011u);
    REQUIRE_EQUALS(instr.rd, 0b00111u);
    REQUIRE_EQUALS(instr.funct3, 0b000u);
    REQUIRE_EQUALS(instr.rs1, 0b11100u);
    REQUIRE_EQUALS(instr.imm, 0b111111111111u);
    REQUIRE_EQUALS(instr.sext_imm(), -0x1);
});
