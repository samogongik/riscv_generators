#include "MyRV32_model.h"




int main() {
    std::vector<int32_t> instructions = {0x00208093,  // addi x1, x1, 2
                                         0x00308113,  // addi x2, x1, 3
                                         0x00410113}; // addi x2, x2, 4

    RV32I_Processor processor(1024, 3);
    processor.loadInstructionsMemory(instructions);


    processor.execute();


    for (int i = 0; i < 32; ++i) {
        std::cout << "x" << i << ": " << processor.readRegister(i) << std::endl;
    }

    return 0;
}

