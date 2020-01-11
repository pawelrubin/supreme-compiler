#include <iostream>
#include <string>

#include "data.hpp"


/*
 **********************
 *       SYMBOL       * 
 **********************
 */

Symbol::Symbol(integer addr) {
  this->addr = addr;
}

integer Symbol::get_addr() {
  return this->addr;
}


/*
 **********************
 *        DATA        * 
 **********************
 */

bool Data::is_declared(ident id) {
  return this->symbols.count(id) != 0;
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
  this->update_offset(end - start + 1);
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

integer Data::get_IDR(int id /*=0*/) {
  return this->memory_offset + 1 + id;
}

integer Data::get_VLR() {
  return this->memory_offset + 3;
}

bool Data::is_lshift_set() {
  return lshift_set;
}

bool Data::is_rshift_set() {
  return rshift_set;
}

/*
 **********************
 *        ARRAY       * 
 **********************
 */

Array::Array(integer addr, integer start, integer end) : Symbol(addr) {
  this->start = start;
  this->end = end;
}

integer Array::get_addr(integer index) {
  return this->get_addr() - this->start + index;
}

integer Array::get_norm_addr() {
  return this->get_addr() - this->start;
}