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
    integer instruction_count = 0;
    void update_offset(long long);
    
  public:
    integer get_instruction_count();
    bool is_power_of_two(integer);

    void insert_jump_address(integer, integer);
    void insert_jump_address(integer);

    // utilities
    InstructionVector start_code();
    InstructionVector end_code();
    codeList get_code();
    void print_code();
    void fprint_code(std::string);

    // high order operations
    InstructionVector rshift();
    InstructionVector lshift();
    InstructionVector parity_test(TIdentifier*);
    InstructionVector set_sign_bit(TIdentifier*, TIdentifier*, Instruction*);

    // atomic operations
    void atomic(std::string);
    void atomic(std::string, integer);
  
    void get();
    void put();

    void load(integer);
    void store(integer);
    void loadi(integer);
    void storei(integer);

    void add(integer);
    void sub(integer);
    void shift(integer);    
    void inc();
    void dec();

    void jump(integer);
    void jpos(integer);
    void jzero(integer);
    void jneg(integer);
    integer jpos();
    integer jzero();
    integer jneg();
    integer jump();

    void halt();

    // memory managment
    InstructionVector insert_to_acc(integer);
    InstructionVector reset_acc();
};
