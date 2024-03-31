export module emulator;

import <cstdint>;
import <cstddef>;
import <stdexcept>;
import <variant>;
import <cassert>;

import memory;
import instruction;
import test_harness;

export class Emulator {
    uint64_t registers[32] {0};
    Memory memory;
    size_t ip = 0;

    void execute_i_type(Instruction::ITypeInstruction instr) {
        const auto rs1 = this->get_register_signed(instr.rs1);
        auto& rd = this->registers[instr.rd];

        if (instr.opcode == 0b0010011u) {
            if (instr.funct3 == 0x00u) {
                // ADDI
                rd = rs1 + instr.sext_imm();
            } else if (instr.funct3 == 0x04u) {
                // XORI
                rd = rs1 ^ instr.sext_imm();
            } else if (instr.funct3 == 0x06u) {
                // ORI
                rd = rs1 | instr.sext_imm();
            } else if (instr.funct3 == 0x07u) {
                // ANDI
                rd = rs1 & instr.sext_imm();
            } else {
                throw std::runtime_error("Unknown I-type instruction!");
            }
        } else if (instr.opcode == 0b000011) {
            if (instr.funct3 == 0x02u) {
                // LW
                rd = this->memory.load32(rs1 + instr.zext_imm());
            } else {
                throw std::runtime_error("Unknown I-type load instruction!");
            }
        } else {
            throw std::runtime_error("Unknown I-type opcode!");
        }
    }

    void execute_r_type(Instruction::RTypeInstruction instr) {
        assert(instr.opcode == 0b0110011u);

        const auto rs1 = this->get_register_signed(instr.rs1);
        const auto rs2 = this->get_register_signed(instr.rs2);
        auto& rd = this->registers[instr.rd];

        if (instr.funct3 == 0x0u && instr.funct7 == 0x00) {
            // ADD
            rd = rs2 + rs1;
        } else if (instr.funct3 == 0x0u && instr.funct7 == 0x20) {
            // SUB
            rd = rs2 - rs1;
        } else if (instr.funct3 == 0x4u && instr.funct7 == 0x00) {
            // XOR
            rd = rs2 ^ rs1;
        } else if (instr.funct3 == 0x6u && instr.funct7 == 0x00) {
            // OR
            rd = rs2 | rs1;
        } else if (instr.funct3 == 0x7u && instr.funct7 == 0x00) {
            // OR
            rd = rs2 & rs1;
        } else {
            throw std::runtime_error("Unknown instruction!");
        }
    }

    void execute_s_type(Instruction::STypeInstruction instr) {
        assert(instr.opcode == 0b0100011u);

        const auto rs1 = this->get_register_signed(instr.rs1);
        const auto rs2 = this->get_register(instr.rs2);

        if (instr.funct3 == 0x2u) {
            this->memory.store32(rs1+instr.sext_imm(), rs2);
        } else {
            throw std::runtime_error("Unknown instruction!");
        }
    }

    public:
    Emulator(size_t ip, Memory memory) : memory(memory), ip(ip) {}

    uint64_t get_register(size_t index) const {
        assert(index < sizeof(registers)/sizeof(uint64_t));

        // x0 is always zero
        if (index == 0) {
            return 0;
        }

        return this->registers[index];
    }

    int32_t get_register_signed(size_t index) const {
        assert(index < sizeof(registers)/sizeof(uint64_t));

        // x0 is always zero
        if (index == 0) {
            return 0;
        }

        return this->registers[index];
    }

    const Memory& get_memory() const {
        return this->memory;
    }

    size_t get_ip() const {
        return this->ip;
    }

    void step() {
        const auto next_instruction = Instruction::parse(memory.load32(this->ip));
        this->ip += 4;

        if (std::holds_alternative<Instruction::ITypeInstruction>(next_instruction)) {
            this->execute_i_type(std::get<Instruction::ITypeInstruction>(next_instruction));
        } else if (std::holds_alternative<Instruction::RTypeInstruction>(next_instruction)) {
            this->execute_r_type(std::get<Instruction::RTypeInstruction>(next_instruction));
        } else if (std::holds_alternative<Instruction::STypeInstruction>(next_instruction)) {
            this->execute_s_type(std::get<Instruction::STypeInstruction>(next_instruction));
        } else {
            throw std::runtime_error("Unhandled instruction type");
        }
    }
};
