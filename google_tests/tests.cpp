#include <gtest/gtest.h>
#include <stdexcept>
#include "../MyRV32_model.h"



TEST(R_type, ADD){
    RV32I_Processor processor(1024, 1);
    processor.writeRegister(3, 20);
    processor.writeRegister(2, 10);

    std::vector<int32_t> instructions = {0b00000000001100010000000010110011}; // ADD rd=1, rs1=2, rs2=3
    processor.loadInstructionsMemory(instructions);
    processor.execute();

    int answer = processor.readRegister(1);
    assert(answer == 30);
}

TEST(R_type, SUB){
    RV32I_Processor processor(1024, 1);
    processor.writeRegister(2, 10);
    processor.writeRegister(3, -20);

    std::vector<int32_t>instructions = {0b01000000001100010000000010110011};
    processor.loadInstructionsMemory(instructions);
    processor.execute();

    int answer = processor.readRegister(1);
    assert(answer == 30);
}


TEST(LoadWordTest, LoadWordInstruction) {
    RV32I_Processor processor(4096, 1);
    processor.writeMemory(3, 12345678);

    std::vector<int32_t> instr = {0b00000000001100010000000100000011};
    processor.loadInstructionsMemory(instr);
    processor.execute();


    EXPECT_EQ(processor.readRegister(2), 12345678);
}

TEST(LoadWordTest, LoadHalfWordInstruction) {
    RV32I_Processor processor(4096, 1);
    processor.writeMemory(3, 65535);

    std::vector<int32_t> instr = {0b0000000001100010001000010000011};
    processor.loadInstructionsMemory(instr);
    processor.execute();


    EXPECT_EQ(processor.readRegister(1), -1);
}

TEST(LoadWordTest, LoadByteInstruction) {
    RV32I_Processor processor(4096, 1);
    processor.writeMemory(3, 255);

    std::vector<int32_t> instr = {0b0000000001100010010000010000011};
    processor.loadInstructionsMemory(instr);
    processor.execute();


    EXPECT_EQ(processor.readRegister(1), -1);
}

TEST(I_type_Immediat_Test, ADDInstruction) {
    RV32I_Processor processor(4096, 1);
    processor.writeRegister(2, 5);

    std::vector<int32_t> instr = {0b0000000001100010000000010010011};
    processor.loadInstructionsMemory(instr);
    processor.execute();


    EXPECT_EQ(processor.readRegister(1), 8);
}


TEST(I_type_Immediat_Test, Logic_and_Instruction) {
    RV32I_Processor processor(4096, 1);
    processor.writeRegister(2, 5);

    std::vector<int32_t> instr = {0b0000000001100010111000010010011};
    processor.loadInstructionsMemory(instr);
    processor.execute();


    EXPECT_EQ(processor.readRegister(1), 1);
}

TEST(I_type_Immediat_Test, Logic_or_Instruction) {
    RV32I_Processor processor(4096, 1);
    processor.writeRegister(2, 5);

    std::vector<int32_t> instr = {0b0000000001100010110000010010011};
    processor.loadInstructionsMemory(instr);
    processor.execute();


    EXPECT_EQ(processor.readRegister(1), 7);
}

TEST(S_type_Test, StoreWordMemory){
    RV32I_Processor processor(4096, 1);
    processor.writeRegister(1, 3);
    processor.writeRegister(2, 100);

    std::vector<int32_t> instr = {0b00000000001000001010001110100011};
    processor.loadInstructionsMemory(instr);
    processor.execute();

    EXPECT_EQ(processor.readMemory(10), 100);
}

TEST(S_type_Test, StoreHalfWordMemory){
    RV32I_Processor processor(4096, 1);
    processor.writeRegister(1, 3);
    processor.writeRegister(2, 165535);

    std::vector<int32_t> instr = {0b00000000001000001001001110100011};
    processor.loadInstructionsMemory(instr);
    processor.execute();

    EXPECT_EQ(processor.readMemory(10), 34463);
}

TEST(S_type_Test, StoreByteMemory){
    RV32I_Processor processor(4096, 1);
    processor.writeRegister(1, 3);
    processor.writeRegister(2, 1000);

    std::vector<int32_t> instr = {0b00000000001000001000001110100011};
    processor.loadInstructionsMemory(instr);
    processor.execute();

    EXPECT_EQ(processor.readMemory(10), 232);
}


TEST(U_type_Test, LUI){
    RV32I_Processor processor(1024, 1);
    processor.writeRegister(2, 4);
    std::vector<int32_t> instr = {0b00000000000000000010000100110111};

    processor.loadInstructionsMemory(instr);
    processor.execute();

    EXPECT_EQ(processor.readRegister(2), 8192);

}

TEST(U_type_Test, AUIPC){
    RV32I_Processor processor(1024, 2);
    processor.writeRegister(2, 4);
    std::vector<int32_t> instr = {0b00000000001100010000000010110011, 0b00000000000000000010000100010111, 0b01000000001100010000000010110011};

    processor.loadInstructionsMemory(instr);
    processor.execute();

    EXPECT_EQ(processor.readRegister(2), 8193);

}

TEST(J_type_test, JAL){
    RV32I_Processor processor(1024, 3);
    processor.writeRegister(3, 20);
    processor.writeRegister(2, 10);
    std::vector<int32_t> instr = {0b00000000001000000000001011101111, 0b00000000001100010000000010110011, 0b01000000001100010000000010110011};

    processor.loadInstructionsMemory(instr);
    processor.execute();

    EXPECT_EQ(processor.readRegister(1), -10);
}


TEST(J_type_test, JALR){
    RV32I_Processor processor(1024, 3);
    processor.writeRegister(3, 20);
    processor.writeRegister(2, 10);
    processor.writeRegister(7, 1);
    std::vector<int32_t> instr = {0b00000000000100111000001011100111, 0b00000000001100010000000010110011, 0b01000000001100010000000010110011};

    processor.loadInstructionsMemory(instr);
    processor.execute();

    EXPECT_EQ(processor.readRegister(1), -10);
}

TEST(B_type_test, BEQ){
    RV32I_Processor processor(1024, 3);
    processor.writeRegister(3, 20);
    processor.writeRegister(2, 10);
    processor.writeRegister(1, 10);

    std::vector<int32_t> instr = {0b00000000001000001000001001100011, 0b00000000001100010000000010110011, 0b01000000001100010000000010110011};

    processor.loadInstructionsMemory(instr);
    processor.execute();

    EXPECT_EQ(processor.readRegister(1), -10);

}

TEST(B_type_test, BNE){
    RV32I_Processor processor(1024, 3);
    processor.writeRegister(3, 20);
    processor.writeRegister(2, 10);
    processor.writeRegister(1, 8);

    std::vector<int32_t> instr = {0b00000000001000001001001001100011, 0b00000000001100010000000010110011, 0b01000000001100010000000010110011};

    processor.loadInstructionsMemory(instr);
    processor.execute();

    EXPECT_EQ(processor.readRegister(1), -10);

}


TEST(B_type_test, BLT){
    RV32I_Processor processor(1024, 3);
    processor.writeRegister(3, 20);
    processor.writeRegister(2, 10);
    processor.writeRegister(1, 8);

    std::vector<int32_t> instr = {0b00000000001000001100000011100011, 0b00000000001100010000000010110011, 0b01000000001100010000000010110011};

    processor.loadInstructionsMemory(instr);
    processor.execute();

    EXPECT_EQ(processor.readRegister(1), -10);

}


TEST(B_type_test, BGE){
    RV32I_Processor processor(1024, 3);
    processor.writeRegister(3, 20);
    processor.writeRegister(2, 10);
    processor.writeRegister(1, 10);

    std::vector<int32_t> instr = {0b00000000001000001111000011100011, 0b00000000001100010000000010110011, 0b01000000001100010000000010110011};

    processor.loadInstructionsMemory(instr);
    processor.execute();

    EXPECT_EQ(processor.readRegister(1), -10);

}

TEST(B_type_test, BLTU){
    RV32I_Processor processor(1024, 3);
    processor.writeRegister(3, 20);
    processor.writeRegister(2, -10);
    processor.writeRegister(1, 10);

    std::vector<int32_t> instr = {0b00000000001000001110000011100011, 0b00000000001100010000000010110011, 0b01000000001100010000000010110011};

    processor.loadInstructionsMemory(instr);
    processor.execute();

    EXPECT_EQ(processor.readRegister(1), -30);

}

TEST(B_type_test, BGEU){
    RV32I_Processor processor(1024, 3);
    processor.writeRegister(3, 20);
    processor.writeRegister(2, -10);
    processor.writeRegister(1, -5);

    std::vector<int32_t> instr = {0b00000000001000001111000011100011, 0b00000000001100010000000010110011, 0b01000000001100010000000010110011};

    processor.loadInstructionsMemory(instr);
    processor.execute();

    EXPECT_EQ(processor.readRegister(1), -30);

}

TEST(Exseptions_test, x0){
    RV32I_Processor processor(1024);

    try {
        processor.writeRegister(0, 20);
    } catch (const std::runtime_error &e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
}

TEST(Exseptions_test, opcode){
    RV32I_Processor processor(1024, 1);
    processor.writeRegister(3, 20);
    processor.writeRegister(2, 10);

    std::vector<int32_t> instr = {0b00000000001100010000000011111111}; // ADD rd=1, rs1=2, rs2=3
    processor.loadInstructionsMemory(instr);

    try {
        processor.execute();
    } catch (const std::runtime_error &e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
}

TEST(Exseptions_test, funct7_for_R_type){
    RV32I_Processor processor(1024, 1);
    processor.writeRegister(3, 20);
    processor.writeRegister(2, 10);

    std::vector<int32_t> instr = {0b001111110001100010000000010110011}; // ADD rd=1, rs1=2, rs2=3
    processor.loadInstructionsMemory(instr);

    try {
        processor.execute();
    } catch (const std::runtime_error &e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
}

TEST(Exseptions_test, funct3_for_I_type) {
    RV32I_Processor processor(4096, 1);
    processor.writeMemory(3, 12345678);

    std::vector<int32_t> instr = {0b00000000001100010111000100000011};
    processor.loadInstructionsMemory(instr);

    try {
        processor.execute();
    } catch (const std::runtime_error &e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }

}

TEST(Exseptions_test, funct3_for_I_type_Immediate) {
    RV32I_Processor processor(4096, 1);
    processor.writeRegister(2, 5);

    std::vector<int32_t> instr = {0b0000000001100010011000010010011};
    processor.loadInstructionsMemory(instr);

    try {
        processor.execute();
    } catch (const std::runtime_error &e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }
}

TEST(Exseptions_test,  funct3_for_S_type){
    RV32I_Processor processor(4096, 1);
    processor.writeRegister(1, 3);
    processor.writeRegister(2, 100);

    std::vector<int32_t> instr = {0b00000000001000001111001110100011};
    processor.loadInstructionsMemory(instr);

    try {
        processor.execute();
    } catch (const std::runtime_error &e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
    }

}