#include "code.hpp"

#include <iostream>
#include <cmath>

// p(0) = 0; p(1) = 1;
void Code::start_code() {
  this->code.push_back("SUB 0");
  this->code.push_back("INC");
  this->code.push_back("STORE 1");
  this->update_offset(1);
}

void Code::end_code() {
  this->code.push_back("HALT");
  this->update_offset(1);
}

codeList Code::get_code() {
  return this->code;
}

void Code::print_code() {
  for (auto &line : this->code) {
    std::cerr << line << std::endl;
  }
}

void Code::update_offset(integer value) {
  this->code_offset += value;
}

void Code::assign(TIdentifier *identifier, TExpression *expr) {
  this->reset_acc();
  std::cerr << "Would assign sth in this memory address: " << identifier->get_mem_addr() << std::endl;
  // if (value > 0) {
  //   for (int i = 0; i < value; i++) {
  //     this->inc();
  //   }
  // } else {
  //   for (int i = value; i > 0; i--) {
  //     this->dec();
  //   }
  // }
  // this->store(variable->get_offset());
}

integer next_power_of_two_exponent(integer n) {
  return ceil(log2(n));
}

void Code::inc() {
  this->code.push_back("INC");
  this->update_offset(1);
}

void Code::dec() {
  this->code.push_back("DEC");
  this->update_offset(1);
}

void Code::store(integer i) {
  this->code.push_back("STORE " + i);
  this->update_offset(1);
}

void Code::load(integer i) {
  this->code.push_back("LOAD " + i);
  this->update_offset(1);
}

void Code::reset_acc() {
  this->code.push_back("SUB 0");
  this->update_offset(1);
}

