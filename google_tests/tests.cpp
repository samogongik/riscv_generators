#include <gtest/gtest.h>
#include "../MyRV32_model.h"



TEST(R_type, ADD){
    RV32I_Processor processor(1024);
    processor.writeRegister(3, 20);
    processor.writeRegister(2, 10);

    std::vector<int32_t> instructions = {0b00000000001100010000000010110011}; // ADD rd=1, rs1=2, rs2=3
    processor.execute(instructions);

    int answer = processor.readRegister(1);
    assert(answer == 30);
}

TEST(R_type, SUB){
    RV32I_Processor processor(1024);
    processor.writeRegister(2, 10);
    processor.writeRegister(3, -20);

    std::vector<int32_t>instructions = {0b01000000001100010000000010110011}; // MUL rd=1, rs1=2, rs2=3
    processor.execute(instructions);

    int answer = processor.readRegister(1);
    assert(answer == 30);
}

TEST(R_type, MUL){
    RV32I_Processor processor(1024);
    processor.writeRegister(2, 5);
    processor.writeRegister(3, 6);

    std::vector<int32_t> instructions = {0b00000010001100010000000010110011}; // MUL rd=1, rs1=2, rs2=3
    processor.execute(instructions);

    int answer = processor.readRegister(1);
    assert(answer == 30);
}

TEST(R_type, DIV){
    RV32I_Processor processor(1024);
    processor.writeRegister(2, 30);
    processor.writeRegister(3, 5);

    std::vector<int32_t> instructions = {0b00000010001100010100000010110011}; // DIV rd=1, rs1=2, rs2=3
    processor.execute(instructions);

    int answer = processor.readRegister(1);
    assert(answer == 6);

}

TEST(LoadWordTest, LoadWordInstruction) {
    RV32I_Processor processor(4096);
    processor.writeMemory(3, 12345678);

    std::vector<int32_t> instr = {0b00000000001100010000000100000011};
    processor.execute(instr);


    EXPECT_EQ(processor.readRegister(2), 12345678);
}

TEST(LoadWordTest, LoadHalfWordInstruction) {
    RV32I_Processor processor(4096);
    processor.writeMemory(3, 65535);

    std::vector<int32_t> instr = {0b0000000001100010001000010000011};
    processor.execute(instr);


    EXPECT_EQ(processor.readRegister(1), -1);
}

TEST(LoadWordTest, LoadByteInstruction) {
    RV32I_Processor processor(4096);
    processor.writeMemory(3, 255);

    std::vector<int32_t> instr = {0b0000000001100010010000010000011};
    processor.execute(instr);


    EXPECT_EQ(processor.readRegister(1), -1);
}

TEST(I_type_Immediat_Test, ADDInstruction) {
    RV32I_Processor processor(4096);
    processor.writeRegister(2, 5);

    std::vector<int32_t> instr = {0b0000000001100010000000010010011};
    processor.execute(instr);


    EXPECT_EQ(processor.readRegister(1), 8);
}


TEST(I_type_Immediat_Test, Logic_and_Instruction) {
    RV32I_Processor processor(4096);
    processor.writeRegister(2, 5);

    std::vector<int32_t> instr = {0b0000000001100010111000010010011};
    processor.execute(instr);


    EXPECT_EQ(processor.readRegister(1), 1);
}

TEST(I_type_Immediat_Test, Logic_or_Instruction) {
    RV32I_Processor processor(4096);
    processor.writeRegister(2, 5);

    std::vector<int32_t> instr = {0b0000000001100010110000010010011};
    processor.execute(instr);


    EXPECT_EQ(processor.readRegister(1), 7);
}

TEST(S_type_Test, LoadWordMemory){
    RV32I_Processor processor(4096);
    processor.writeRegister(1, 3);
    processor.writeRegister(2, 100);

    std::vector<int32_t> instr = {0b00000000001000001010001110100011};
    processor.execute(instr);

    EXPECT_EQ(processor.readMemory(10), 100);
}

TEST(S_type_Test, LoadHalfWordMemory){
    RV32I_Processor processor(4096);
    processor.writeRegister(1, 3);
    processor.writeRegister(2, 165535);

    std::vector<int32_t> instr = {0b00000000001000001001001110100011};
    processor.execute(instr);

    EXPECT_EQ(processor.readMemory(10), 34463);
}

TEST(S_type_Test, LoadByteMemory){
    RV32I_Processor processor(4096);
    processor.writeRegister(1, 3);
    processor.writeRegister(2, 1000);

    std::vector<int32_t> instr = {0b00000000001000001000001110100011};
    processor.execute(instr);

    EXPECT_EQ(processor.readMemory(10), 232);
}

TEST(UJ_type_Test, Jump){
    RV32I_Processor processor(4096);

    std::vector<int32_t> instr = {0b00000000000001100100000011101111};
    processor.execute(instr);

    EXPECT_EQ(processor.readRegister(1), 100);
}







