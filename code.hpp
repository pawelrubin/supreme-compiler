#pragma once

#include "data.hpp"
#include "typedefs.hpp"
#include "types.hpp"

#include <string>

class Code {
  private:
    codeList code;
    long long instruction_count = 0;
    void update_offset(long long value);

  public:
    // utilities
    void start_code();
    void end_code();
    codeList get_code();
    void print_code();
    void fprint_code(std::string filename);

    // high order operations
    void assign(TIdentifier *identifier, TExpression *expr);
    void read(TIdentifier *identifier);
    void write(TValue *value);

    // atomic operations
    void atomic(std::string instr);
    void atomic(std::string instr, integer i);
  
    void get();
    void put();

    void load(integer i);
    void store(integer i);
    void loadi(integer i);
    void storei(integer i);

    void add(integer i);
    void sub(integer i);
    void shift(integer i);    
    void inc();
    void dec();

    void jump(integer j);
    void jpos(integer j);
    void jzero(integer j);
    void jneg(integer j);

    void halt();

    // memory managment
    void insert_to_acc(integer value);
    void reset_acc();
};
