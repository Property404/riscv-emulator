export module emulator;

import <cstdint>;
import <cstddef>;
import <iostream>;
import <stdexcept>;
import <variant>;
import <map>;
import <cassert>;
import <functional>;

import memory;
import instruction;
import test_harness;

export class Emulator;
using RegType = uint64_t;

export namespace Register {
    const size_t ZERO = 0;
    const size_t RA = 1;
    const size_t SP = 2;
    const size_t T0 = 5;
    const size_t T1 = 6;
    const size_t T2 = 7;
    const size_t FP = 8;
    const size_t ARG0 = 10;
    const size_t ARG1 = 11;
    const size_t ARG2 = 12;
    const size_t ARG5 = 15;
    const size_t ARG6 = 16;
    const size_t ARG7 = 17;
    const size_t T3 = 28;
    const size_t T4 = 29;
    const size_t T5 = 30;
    const size_t T6 = 31;
};

class Emulator {
    using ECall = std::function<void(Emulator&)>;
    RegType registers[32] {0};
    Memory memory;
    bool exited = false;
    size_t ip = 0;
    std::map<RegType, ECall> ecalls{};

    void execute_i_type(const Instruction::ITypeInstruction instr) {
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
            } else if (instr.funct3 == 0x01u && ((instr.imm >> 6) == 0x00)) {
                // SLLI
                rd = rs1 << (instr.imm & 0x3F);
            } else if (instr.funct3 == 0x05u && ((instr.imm >> 6) == 0x00)) {
                // SRLI
                rd = rs1 >> (instr.imm & 0x3F);
            } else {
                throw std::runtime_error("Unknown I-type instruction!");
            }
        } else if (instr.opcode == 0b0011011u) {
            if (instr.funct3 == 0x00u) {
                // ADDIW
                rd &= 0xFFFFFFFF'00000000;
                rd |= (uint32_t)(rs1) + (uint32_t)(instr.sext_imm());
            } else {
                throw std::runtime_error("Unknown I-type instruction!");
            }
        } else if (instr.opcode == 0b000011) {
            if (instr.funct3 == 0x02u) {
                // LW
                rd = (int32_t)(this->memory.load32(rs1 + instr.sext_imm()));
            } else if (instr.funct3 == 0x03u) {
                // LD
                rd = (int64_t)(this->memory.load64(rs1 + instr.sext_imm()));
            } else if (instr.funct3 == 0x04u) {
                // LBU
                rd = this->memory.load8(rs1 + instr.sext_imm());
            } else if (instr.funct3 == 0x06u) {
                // LWU
                rd = this->memory.load32(rs1 + instr.sext_imm());
            } else {
                throw std::runtime_error("Unknown I-type load instruction: " + std::to_string(instr.funct3));
            }
        } else if (instr.opcode == 0b1110011 && instr.imm == 0) {
            // ECALL
            const auto a7 = this->get_register(17);
            this->ecalls.at(a7)(*this);
        } else if (instr.opcode == 0b1100111 && instr.funct3 == 0) {
            // JALR
            rd = this->ip;
            this->ip = rs1 + instr.sext_imm();
        } else {
            throw std::runtime_error("Unknown I-type instruction!");
        }
    }

    void execute_r_type(Instruction::RTypeInstruction instr) {
        const auto rs1 = this->get_register_signed(instr.rs1);
        const auto rs2 = this->get_register_signed(instr.rs2);
        const auto rs1_unsigned = this->get_register(instr.rs1);
        const auto rs2_unsigned = this->get_register(instr.rs2);
        auto& rd = this->registers[instr.rd];

        if (instr.opcode == 0b0110011u) {
            if (instr.funct3 == 0x0u && instr.funct7 == 0x00) {
                // ADD
                rd = rs2 + rs1;
            } else if (instr.funct3 == 0x0u && instr.funct7 == 0x20) {
                // SUB
                rd = rs1 - rs2;
            } else if (instr.funct3 == 0x4u && instr.funct7 == 0x00) {
                // XOR
                rd = rs2 ^ rs1;
            } else if (instr.funct3 == 0x6u && instr.funct7 == 0x00) {
                // OR
                rd = rs2 | rs1;
            } else if (instr.funct3 == 0x7u && instr.funct7 == 0x00) {
                // AND
                rd = rs2 & rs1;
            } else if (instr.funct3 == 0x3u && instr.funct7 == 0x00) {
                // SLTU
                rd = rs1_unsigned < rs2_unsigned;
            } else {
                throw std::runtime_error("Unknown R-type instruction!");
            }
        } else if(instr.opcode == 0b0111011) {
            if (instr.funct3 == 0x0u && instr.funct7 == 0b0100000) {
                // SUBW TODO: do this conversion properly
                rd = static_cast<int32_t>(rs1 & 0xFFFF'FFFF) -
                    static_cast<int32_t>(rs2 & 0xFFFF'FFFF);
            } else if (instr.funct3 == 0x5u && instr.funct7 == 0b0000000) {
                // SRLW TODO: do this conversion properly
                rd = static_cast<int32_t>(rs1 & 0xFFFF'FFFF) >>
                    static_cast<int32_t>(rs2 & 0xFFFF'FFFF);
            } else {
                throw std::runtime_error("Unknown R-type instruction(W)!");
            }
        } else {
                throw std::runtime_error("Unknown R-type opcode! " + std::to_string(instr.opcode));
        }
    }

    void execute_s_type(Instruction::STypeInstruction instr) {
        assert(instr.opcode == 0b0100011u);

        const auto rs1 = this->get_register_signed(instr.rs1);
        const auto rs2 = this->get_register(instr.rs2);

        if (instr.funct3 == 0x0u) {
            // SB
            this->memory.store8(rs1+instr.sext_imm(), rs2);
        } else if (instr.funct3 == 0x2u) {
            // SW
            this->memory.store32(rs1+instr.sext_imm(), rs2);
        } else if (instr.funct3 == 0x3u) {
            // SD
            this->memory.store64(rs1+instr.sext_imm(), rs2);
        } else {
            throw std::runtime_error("Unknown S-type instruction!");
        }
    }

    void execute_u_type(Instruction::UTypeInstruction instr) {
        auto& rd = this->registers[instr.rd];

        if (instr.opcode == 0b0010111u) {
            // AUIPC
            rd = (this->ip - 4) + instr.zext_imm();
        } else if (instr.opcode == 0b0110111u) {
            // LUI
            rd = instr.zext_imm();
        } else {
            throw std::runtime_error("Unknown U-type instruction!");
        }
    }

    void execute_b_type(Instruction::BTypeInstruction instr) {
        assert(instr.opcode == 0b1100011);
        const auto rs1 = this->get_register(instr.rs1);
        const auto rs2 = this->get_register(instr.rs2);

        if (instr.funct3 == 0x0) {
            // BEQ
            if (rs1 == rs2) {
                this->ip += instr.sext_imm() - 4;
            }
        } else if (instr.funct3 == 0x1) {
            // BNE
            if (rs1 != rs2) {
                this->ip += instr.sext_imm() - 4;
            }
        } else if (instr.funct3 == 0x6) {
            // BLTU
            if (rs1 < rs2) {
                this->ip += instr.sext_imm() - 4;
            }
        } else if (instr.funct3 == 0x5) {
            // BGE
            if (rs1 >= rs2) {
                this->ip += instr.sext_imm() - 4;
            }
        } else {
            throw std::runtime_error("Unknown B-type instruction!");
        }
    }

    void execute_j_type(Instruction::JTypeInstruction instr) {
        auto& rd = this->registers[instr.rd];

        if (instr.opcode == 0b1101111) {
            // JAL
            rd = this->ip;
            this->ip += instr.sext_imm() - 4;
        } else {
            throw std::runtime_error("Unknown J-type instruction!");
        }
    }

    public:
    Emulator(size_t ip, Memory memory) : memory(memory), ip(ip) {
    }

    RegType get_register(size_t index) const {
        assert(index < sizeof(registers)/sizeof(RegType));

        // x0 is always zero
        if (index == 0) {
            return 0;
        }

        return this->registers[index];
    }

    int32_t get_register_signed(size_t index) const {
        assert(index < sizeof(registers)/sizeof(RegType));

        // x0 is always zero
        if (index == 0) {
            return 0;
        }

        return this->registers[index];
    }

    const Memory& get_memory() const {
        return this->memory;
    }

    Memory& get_memory_mut() {
        return this->memory;
    }

    void register_ecall(RegType num, ECall&& call) {
        this->ecalls[num] = call;
    }

    size_t get_ip() const {
        return this->ip;
    }

    uint32_t current_instruction() const {
        return memory.load32(this->ip);
    }

    void step() {
        if (this->exited) {
            throw std::runtime_error("Stepped after program exit");
        }

        const auto next_instruction = Instruction::parse(memory.load32(this->ip));
        this->ip += 4;

        if (std::holds_alternative<Instruction::ITypeInstruction>(next_instruction)) {
            this->execute_i_type(std::get<Instruction::ITypeInstruction>(next_instruction));
        } else if (std::holds_alternative<Instruction::RTypeInstruction>(next_instruction)) {
            this->execute_r_type(std::get<Instruction::RTypeInstruction>(next_instruction));
        } else if (std::holds_alternative<Instruction::STypeInstruction>(next_instruction)) {
            this->execute_s_type(std::get<Instruction::STypeInstruction>(next_instruction));
        } else if (std::holds_alternative<Instruction::UTypeInstruction>(next_instruction)) {
            this->execute_u_type(std::get<Instruction::UTypeInstruction>(next_instruction));
        } else if (std::holds_alternative<Instruction::BTypeInstruction>(next_instruction)) {
            this->execute_b_type(std::get<Instruction::BTypeInstruction>(next_instruction));
        } else if (std::holds_alternative<Instruction::JTypeInstruction>(next_instruction)) {
            this->execute_j_type(std::get<Instruction::JTypeInstruction>(next_instruction));
        } else {
            throw std::runtime_error("Unhandled instruction type");
        }
    }

    bool has_exited() const {
        return this->exited;
    }

    void exit() {
        this->exited = true;
    }

    void run() {
        while(! this->exited) {
            step();
        }
    }
};
