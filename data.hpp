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
  G,
  Count
};

class Symbol {
  protected:
    integer addr;

  public:
    explicit Symbol(integer addr);
    Symbol() = default;
    virtual integer get_addr();
    void set_addr(integer a) {this->addr = a;}; 
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

    integer iterators_count = 0;

    void update_offset(integer);

  public:
    bool is_declared(ident);
    integer get_register(Register);
    void declare_variable(ident);
    void declare_array(ident, integer, integer);
    Variable* new_iterator(ident);

    Variable* declare_bad_variable(ident);


    void inc_iterator_count() {iterators_count++;}

    void del_iterator(ident);
    void print_symbols();
    Symbol* get_symbol(ident);
};
