#pragma once

#include "typedefs.hpp"

#include <unordered_map>
#include <string>


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

class Data {
  private:
    integer memory_offset = 1; // p(0) = ACC
    std::unordered_map<ident, Symbol*> symbols;
    
    bool is_declared(ident id);
    void update_offset(integer value);

  public:
    integer get_IDR(); // get IDR addr
    integer get_val_reg_addr(); // second cell after declarations
    void declare_variable(ident id);
    void declare_array(ident id, integer start, integer end);
    void print_symbols();
    Symbol* get_symbol(ident id);
};

