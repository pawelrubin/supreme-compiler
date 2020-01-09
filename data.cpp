#include <iostream>
#include <string>

#include "data.hpp"

bool Data::is_declared(ident id) {
  return this->symbols.count(id) != 0;
}

Symbol::Symbol(integer addr) {
  this->addr = addr;
}

integer Symbol::get_addr() {
  return this->addr;
}

void Data::update_offset(integer value) {
  this->memory_offset += value;
}

void Data::declare_variable(ident id) {
  if (this->is_declared(id)) throw std::string(id + " already declared.");
  this->symbols[id] = new Variable(this->memory_offset);
  this->update_offset(1);
}

void Data::declare_array(ident id, integer start, integer end) {
  if (this->is_declared(id)) throw std::string(id + " already declared.");
  this->symbols[id] = new Array(this->memory_offset, start, end);
  this->update_offset(end - start);
}

Array::Array(integer addr, integer start, integer end) : Symbol(addr) {
  this->start = start;
  this->end = end;
}

integer Array::get_addr(integer index) {
  return this->get_addr() - this->start + index;
}

void Data::print_symbols() {
  for (auto &[k, v] : this->symbols) {
    std::cerr << "Symbol name is " << k << " and its addr = " << v->get_addr() << std::endl;
  }
}

Symbol* Data::get_symbol(ident id) {
  if (!this->is_declared(id)) throw std::string("Variable " + id + " hasn't been declared.");
  return this->symbols[id];
}
