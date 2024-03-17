#include <iostream>
#include <vector>
#include <array>

class RV32I_RegisterFile final{
private:
    std::array<int32_t, 32> regs;

public:
    RV32I_RegisterFile() : regs({}) {
        for (auto elem : regs){
            elem = 0;
        }
    }

    int32_t read(int reg_num) const noexcept{
        return regs[reg_num];
    }

    void write(int reg_num, int32_t value){
        if (reg_num != 0)
            regs[reg_num] = value;
        else {
            throw std::runtime_error ("attempt to change value x0");
        }
    }
};

class RV32I_Memory final{
private:
    std::vector<int32_t> mem;

public:
    RV32I_Memory(int size) : mem(size, 0) {}

    int32_t read(int32_t address) const noexcept {
        return mem[address];
    }

    void write(int32_t address, int32_t value) noexcept{
        mem[address] = value;
    }
};

class RV32I_Processor final{
private:
    RV32I_RegisterFile regfile;
    RV32I_Memory memory;
    int32_t pc;
    int32_t _amountInstructions = 0;

public:
    RV32I_Processor(int mem_size, int32_t amountInstructions = 0) : memory(mem_size), pc(0), _amountInstructions(amountInstructions) {}

    void loadInstructionsMemory (const std::vector<int32_t>& instr) noexcept {
        for (int i = 0; i < instr.size(); i++){
            memory.write(i, instr[i]);
        }
    }

    int32_t readRegister(int reg_num) const noexcept{
        return regfile.read(reg_num);
    }

    void writeRegister(int reg_num, int value) {
        regfile.write(reg_num, value);
    }

    int32_t readMemory(int32_t address) const noexcept{
        return memory.read(address);
    }

    void writeMemory(int32_t address, int32_t value) noexcept{
        memory.write(address, value);
    }

    void execute() {
        while (pc < _amountInstructions) {
            bool flagJump = false;
            int opcode = readMemory(pc) & 0x7F;
            int rd = (readMemory(pc) >> 7) & 0x1F;
            int rs1 = (readMemory(pc) >> 15) & 0x1F;
            int rs2 = (readMemory(pc) >> 20) & 0x1F;

            int funct7 = (readMemory(pc) >> 25) & 0x7F;
            int funct3 = (readMemory(pc) >> 12) & 0x7;
            int32_t imm = 0;

            switch (opcode) {
                case 0b0110011: // R-type
                    switch (funct7) {

                        case 0b0000000:
                            regfile.write(rd, regfile.read(rs1) + regfile.read(rs2));
                            break;

                        case 0b0100000:
                            regfile.write(rd, regfile.read(rs1) - regfile.read(rs2));
                            break;

                        default:  throw std::runtime_error ("unknown funct7 for R-type instruction = " + std::to_string(funct7));
                    }
                    break;

                case  0b0000011: // I-type - Load
                    int32_t load_address;
                    imm = readMemory(pc) >> 20;
                    load_address = regfile.read(rs1) + imm;


                    switch (funct3) {

                        case 0b000:
                            regfile.write(rd, memory.read(load_address));
                            break;

                        case 0b001:
                            int16_t halfword;
                            halfword = memory.read(load_address);

                            if (halfword & 0x8000) {
                                halfword |= 0xFFFF0000;
                            }
                            regfile.write(rd, halfword);
                            break;

                        case 0b010:
                            int32_t byte;
                            byte = memory.read(load_address);

                            if (byte & 0x80) {
                                byte |= 0xFFFFFF00;
                            }
                            regfile.write(rd, byte);
                            break;

                        default:  throw std::runtime_error ("unknown func3 for I-type instruction = " + std::to_string(funct3));
                    }
                    break;


                case 0b0010011: // I-type - Immediate
                    imm = readMemory(pc) >> 20;
                    switch (funct3) {

                        case 0b000:
                            regfile.write(rd, regfile.read(rs1) + imm);
                            break;

                        case 0b111:
                            regfile.write(rd, regfile.read(rs1) & imm);
                            break;

                        case 0b110:
                            regfile.write(rd, regfile.read(rs1) | imm);
                            break;

                        default:  throw std::runtime_error ("unknown funct3 for I-type - Immediate = " + std::to_string(funct3));
                    }
                    break;

                case 0b0100011: //S-type
                    int32_t store_address, store_data;
                    imm = ((readMemory(pc) >> 25) << 5) | ((readMemory(pc) >> 7) & 0x1F);
                    store_address = regfile.read(rs1) + imm;
                    store_data = regfile.read(rs2);

                    switch (funct3) {
                        case 0b000:
                            memory.write(store_address, store_data & 0xFF);
                            break;

                        case 0b001:
                            memory.write(store_address, store_data & 0xFFFF);
                            break;

                        case 0b010:
                            memory.write(store_address, store_data);
                            break;

                        default:
                            throw std::runtime_error("unknown funct3 for S-type instruction = " + std::to_string(funct3));
                    }
                    break;



                case 0b0110111: // LUI
                    rd = (readMemory(pc) >> 7) & 0x1F;
                    imm = readMemory(pc) & 0xFFFFF000;
                    regfile.write(rd, imm);
                    break;

                case 0b0010111: // AUIPC
                    rd = (readMemory(pc) >> 7) & 0x1F;
                    imm = readMemory(pc) & 0xFFFFF000;
                    imm += pc;
                    regfile.write(rd, imm);
                    break;

                case 0b1101111: // JAL
                    rd = (readMemory(pc) >> 7) & 0x1F;
                    imm = ((readMemory(pc) & 0x80000000) ? 0xFFF00000 : 0x0) |
                          ((readMemory(pc) >> 20) & 0xFF) |
                          ((readMemory(pc) >> 9) & 0x800) |
                          ((readMemory(pc) >> 10) & 0x7FE);

                    regfile.write(rd, pc + 1);
                    pc += imm;
                    flagJump = true;
                    break;

                case 0b1100111: // JALR
                    rd = (readMemory(pc) >> 7) & 0x1F;
                    rs1 = (readMemory(pc) >> 15) & 0x1F;

                    imm = ((readMemory(pc) & 0x80000000) ? 0xFFF00000 : 0x0) |
                          ((readMemory(pc) >> 20) & 0xFFF); // Bits 11:0

                    int32_t address;
                    address = regfile.read(rs1) + imm;
                    regfile.write(rd, pc + 1);
                    pc = address;
                    flagJump = true;
                    break;

                case 0b1100011: // B-type
                    int32_t branch_target;
                    imm = ((readMemory(pc) & 0x80000000) ? 0xFFFFF000 : 0x0) |
                          ((readMemory(pc) >> 8) & 0b1111) |
                          ((readMemory(pc) >> 25) & 0b111111) |
                          ((readMemory(pc) >> 7) & 0b1);

                    branch_target = pc + imm;
                    flagJump = true;

                    switch (funct3) {
                        case 0b000: // BEQ
                            if (regfile.read(rs1) == regfile.read(rs2)) {
                                pc = branch_target;

                            }
                            break;

                        case 0b001: // BNE
                            if (regfile.read(rs1) != regfile.read(rs2)) {
                                pc = branch_target;
                            }
                            break;

                        case 0b100: // BLT
                            if (regfile.read(rs1) < regfile.read(rs2)) {
                                pc = branch_target;
                            }
                            break;

                        case 0b101: // BGE
                            if (regfile.read(rs1) >= regfile.read(rs2)) {
                                pc = branch_target;
                            }
                            break;

                        case 0b110: // BLTU
                            if (static_cast<uint32_t>(regfile.read(rs1)) < static_cast<uint32_t>(regfile.read(rs2))) {
                                pc = branch_target;
                            }
                            break;

                        case 0b111: // BGEU
                            if (static_cast<uint32_t>(regfile.read(rs1)) >= static_cast<uint32_t>(regfile.read(rs2))) {
                                pc = branch_target;
                            }
                            break;

                        default:
                            throw std::runtime_error("unknown funct3 for B-type instruction = " + std::to_string(funct3));
                    }
                    break;


                default:  throw std::runtime_error ("unknown opcode = " + std::to_string(opcode));

            }
            if (!flagJump){
                pc++;
            }
        }
    }
};
