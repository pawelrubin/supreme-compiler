#include "code.hpp"

#include <iostream>
#include <cmath>
#include <string>
#include <fstream>

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
  std::cout << std::endl;
  for (auto &line : this->code) {
    std::cout << line << std::endl;
  }
}

void Code::fprint_code(std::string filename) {
  std::ofstream fout(filename);
  for (auto &line : this->code) {
    fout << line << std::endl;
  }
  fout.close();
}

void Code::update_offset(integer value) {
  this->instruction_count += value;
}

integer next_power_of_two_exponent(integer n) {
  return ceil(log2(n));
}

/*
 *************************
 * high order operations *
 *************************
*/

void Code::assign(TIdentifier *identifier, TExpression *expr) {
  if (TArrayVariableIdentifier *id = dynamic_cast<TArrayVariableIdentifier*>(identifier)) {
    id->load_addr_to_idr(); // IDR = id.addr
    expr->load_expr();              // ACC = expr.value.value
    this->storei(data->get_IDR());  // p(IDR) = ACC
  } else {
    expr->load_expr();
    this->store(identifier->get_addr());
  }
}

void Code::write(TValue *value) {
  value->load_value(); // ACC = value.value
  this->put();         // PUT
}

void Code::read(TIdentifier *identifier) {
  identifier->load_addr_to_idr(); // IDR = id.addr
  this->get();                    // ACC = input
  this->storei(data->get_IDR());  // p(IDR) = ACC
}


/*
 ***********
 * atomics *
 ***********
*/

void Code::atomic(std::string instr) {
  this->code.push_back(instr);
  this->update_offset(1);
}

void Code::atomic(std::string instr, integer i) {
  this->code.push_back(instr + " " + std::to_string(i));
  this->update_offset(1);
}

void Code::get() {
  this->atomic("GET");
}

void Code::put() {
  this->atomic("PUT");
}

void Code::load(integer i) {
  this->atomic("LOAD", i);
}

void Code::store(integer i) {
  this->atomic("STORE", i);
}

void Code::loadi(integer i) {
  this->atomic("LOADI", i);
}

void Code::storei(integer i) {
  this->atomic("STOREI", i);
}

void Code::add(integer i) {
  this->atomic("ADD", i);
}

void Code::sub(integer i) {
  this->atomic("SUB", i);
}

void Code::shift(integer i) {
  this->atomic("SHIFT", i);
}

void Code::inc() {
  this->atomic("INC");
}

void Code::dec() {
  this->atomic("DEC");
}

void Code::jump(integer j) {
  this->atomic("JUMP", j);
}

void Code::jpos(integer j) {
  this->atomic("JPOS", j);
}

void Code::jzero(integer j) {
  this->atomic("JZEO", j);
}

void Code::jneg(integer j) {
  this->atomic("JNEG", j);
}

void Code::halt() {
  this->atomic("HALT");
}

/*
 *********************
 * memory managmnent *
 *********************
*/

void Code::insert_to_acc(integer value) { // TODO: Optimize constants generation.
  this->reset_acc();
  if (value > 0) {
    for (int i = 0; i < value; i++) {
      this->inc();
    }
  } else {
    for (int i = value; i < 0; i++) {
      this->dec();
    }
  }
}

void Code::reset_acc() {
  this->sub(0);
}
