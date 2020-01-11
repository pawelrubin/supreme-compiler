#pragma once

#include "typedefs.hpp"

#include <unordered_map>
#include <string>


enum class Register {
  IDR,
  IDR1,
  VLR,
  A,
  B,
  C,
  D
};

class Symbol {
  protected:
    integer addr;

  public:
    explicit Symbol(integer addr);
    Symbol() = default;
    virtual integer get_addr();
};

class Variable : public Symbol {
  using Symbol::Symbol;

  private:
    bool is_initialized;  
};

class Array : public Symbol {
  private:
    integer start;
    integer end;

  public:
    using Symbol::get_addr;
    Array(integer addr, integer start, integer end);
    integer get_addr(integer index);
    integer get_norm_addr();
};

/**
 * 
 * Virtual machine memory: [ACC, RSHIFT, LSHIFT, Symbols..., IDR, IDR1, VLR, A, B]
 *
 **/
class Data {
    
  // TODO: keep variables values if possible, then use them while compiling expressions etc
  // TODO: keep values held in ACC and registers to optimize constants generation

  private:
    integer memory_offset = 3; // ACC
    std::unordered_map<ident, Symbol*> symbols;

    bool lshift_set;
    bool rshift_set;

    bool is_declared(ident id);
    void update_offset(integer value);

  public:
    integer get_register(Register);
    void declare_variable(ident id);
    void declare_array(ident id, integer start, integer end);
    void print_symbols();
    Symbol* get_symbol(ident id);

    bool is_lshift_set(); // is p(1) set to 1
    bool is_rshift_set(); // is p(2) set to -1
};
