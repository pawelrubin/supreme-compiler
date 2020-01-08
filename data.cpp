#include <iostream>
#include <string>

#include "data.hpp"

void Data::check_context(ident id) {
  if (this->symbols.count(id) != 0) {
    throw std::string(id + " already defined.");
  };
}

Symbol::Symbol(long long offset) {
  this->offset = offset;
}

long long Symbol::get_offset() {
  return this->offset;
}

void Data::update_offset(long long value) {
  this->memory_offset += value;
}

void Data::declare_variable(ident id) {
  this->check_context(id);
  this->symbols[id] = Variable(this->memory_offset);
  this->update_offset(1);
}

void Data::declare_array(ident id, long long start, long long end) {
  this->check_context(id);
  this->symbols[id] = Array(this->memory_offset, start, end);
  this->update_offset(end - start);
}

Array::Array(long long offset, long long start, long long end) : Symbol(offset) {
  this->start = start;
  this->end = end;
}

void Data::print_symbols() {
  for (auto &[k, v] : this->symbols) {
    std::cerr << "Symbol name is " << k << " and its offset = " << v.get_offset() << ". Memory offset is " << this->memory_offset << std::endl;
  }
}
