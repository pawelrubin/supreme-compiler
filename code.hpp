#pragma once

#include "data.hpp"
#include "typedefs.hpp"
#include "types.hpp"

#include <string>

class Code {
  // IDEA indent output code
  // TODO instructions as list of objects
  private:
    codeList code;
    
  public:
    // utilities
    codeList get_code();
    void print_code();
    void fprint_code(std::string, codeList);
    bool is_power_of_two(integer);

    // high order operations
    InstructionVector start_code();
    InstructionVector end_code();
    InstructionVector rshift();
    InstructionVector lshift();
    InstructionVector parity_test(TIdentifier*);
    InstructionVector set_sign_bit(TIdentifier*, TIdentifier*);
    InstructionVector insert_to_acc(integer);
    InstructionVector reset_acc();
};
