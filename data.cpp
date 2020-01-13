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
  this->update_offset(abs(end - start) + 1);
}

Variable* Data::new_iterator(ident iterator) {
  if (!is_declared(iterator)) {
    this->symbols[iterator] = new Variable(get_register(Register::Count) + iterators_count);
  } else {
    this->symbols[iterator]->set_addr(get_register(Register::Count) + iterators_count);
  }
  iterators_count += 2; // end value stored next to iterator
  return static_cast<Variable*>(this->symbols[iterator]);
}

void Data::del_iterator(ident iterator) {
  this->symbols.erase(iterator);
  iterators_count -= 2;
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

integer Data::get_register(Register reg) { // TODO rename
  return memory_offset + integer(reg) + 1;
}

Variable* Data::declare_bad_variable(ident id) {
  this->symbols[id] = new Variable(-1);
  return static_cast<Variable*>(this->symbols[id]);
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