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
  D,
  E,
  F,
  G
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
    Array(integer, integer, integer);
    integer get_addr(integer);
    integer get_norm_addr();
};

/**
 * 
 * Virtual machine memory: [ACC, LSHIFT, RSHIFT, Symbols..., IDR, IDR1, VLR, A, B, ...]
 *
 **/
class Data {
    
  // TODO: keep variables values if possible, then use them while compiling expressions etc
  // TODO: keep values held in ACC and registers to optimize constants generation
  // TODO: remember what is currently loaded

  private:
    integer memory_offset = 3; // ACC
    std::unordered_map<ident, Symbol*> symbols;

    bool is_declared(ident);
    void update_offset(integer);

  public:
    integer get_register(Register);
    void declare_variable(ident);
    void declare_array(ident, integer, integer);
    void print_symbols();
    Symbol* get_symbol(ident);
};
