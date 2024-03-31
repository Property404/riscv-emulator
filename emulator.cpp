export module emulator;

import <cstdint>;
import <cstddef>;
import <stdexcept>;
import <variant>;

import memory;
import instruction;

export class Emulator {
    uint32_t registers[32];
    Memory memory;
    size_t ip = 0;

    void execute_i_type(Instruction::ITypeInstruction instr) {
        if (instr.opcode == 0b0010011u && instr.funct3 == 0b000u) {
            // ADDI instruction
            this->registers[instr.rd] =
                this->registers[instr.rs1] + instr.sext_imm();
        } else {
            throw std::runtime_error("Unknown instruction!");
        }
    }

    public:
    Emulator(size_t ip, Memory memory);

    void step() {
        const auto next_instruction = Instruction::parse(memory.load32(this->ip));
        this->ip += 4;

        if (std::holds_alternative<Instruction::ITypeInstruction>(next_instruction)) {
            this->execute_i_type(std::get<Instruction::ITypeInstruction>(next_instruction));
        } else {
            throw std::runtime_error("Unhandled instruction type");
        }
    }
};
