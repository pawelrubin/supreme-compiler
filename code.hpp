#pragma once

#include "data.hpp"
#include "typedefs.hpp"
#include "types.hpp"

#include <string>

class Code {

  // IDEA methods should return number of instructions performed
  // IDEA indent output code

  private:
    codeList code;
    integer instruction_count = 0;
    void update_offset(long long);

  public:

    integer get_instruction_count();

    void insert_jump_address(integer, integer);
    void insert_jump_address(integer);

    // utilities
    void start_code();
    void end_code();
    codeList get_code();
    void print_code();
    void fprint_code(std::string);

    // language operations
    void assign(TIdentifier*, TExpression*);
    void read(TIdentifier*);
    void write(TValue*);

    // high order operations
    void parity_test(TIdentifier*);
    void rshift();
    void lshift();
    void set_sign_bit(TIdentifier*, TIdentifier*);

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
    void jump();
    void jpos(integer);
    void jpos();
    void jzero(integer);
    void jzero();
    void jneg(integer);
    void jneg();

    void halt();

    // memory managment
    void insert_to_acc(integer);
    void reset_acc();
    void set_lshift();
    void set_rshift();
};
