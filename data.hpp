#pragma once

#include <unordered_map>
#include <string>

typedef std::string ident;

class Symbol {
  protected:
    long long offset;

  public:
    explicit Symbol(long long offset);
    Symbol() = default;
    long long get_offset();
};

class Variable : public Symbol {
  using Symbol::Symbol;
};

class Array : public Symbol {
  private:
    long long start;
    long long end;

  public:
    Array(long long offset, long long start, long long end);
};

class Data {
  private:
    long long memory_offset = 0;
    std::unordered_map<ident, Symbol> symbols;
    void check_context(ident id);
    void update_offset(long long value);

  public:
    void declare_variable(ident id);
    void declare_array(ident id, long long start, long long end);
    void print_symbols();
};

