#include "code.hpp"

#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <algorithm>

integer Code::get_instruction_count() {
  return instruction_count;
} 

void Code::insert_jump_address(integer k, integer j) {
  std::string& jump = code[k];
  std::size_t pos = jump.find("-1");
  if (pos != std::string::npos)
    jump.replace(pos, 2, std::to_string(j));
}

// sets shifts
void Code::start_code() {
  reset_acc();
  dec();
  store(2);
  inc();
  inc();
  store(1);
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

void Code::parity_test(TIdentifier* id) {
  if (TArrayVariableIdentifier *avid = dynamic_cast<TArrayVariableIdentifier*>(id)) {
    avid->load_value_to_register(Register::IDR);
    id = new TVariableIdentifier(new Variable(data->get_register(Register::IDR)));
  }
  id->load_value_to_acc();
  rshift();
  lshift();
  sub(id->get_addr());
}

void Code::lshift() {
  shift(1);
}

void Code::rshift() {
  shift(2);
}

void Code::set_sign_bit(TIdentifier* lid, TIdentifier* rid) {
  reset_acc(); 
  store(data->get_register(Register::D));
  lid->load_value_to_acc();
  integer j = instruction_count;
  jpos();
    lid->negate(true);
    reset_acc();
    inc();
    store(data->get_register(Register::D));
  insert_jump_address(j, instruction_count);

  rid->load_value_to_acc();
  j = instruction_count;
  jpos();
    rid->negate(true);
    load(data->get_register(Register::D));
    jzero(3);
      dec();
      jump(instruction_count + 2);
    inc();
    store(data->get_register(Register::D));
  insert_jump_address(j, instruction_count);
}

/*
 ***********************
 * language operations *
 ***********************
*/

// assign
void Code::assign(TIdentifier *identifier, TExpression *expr) {
  if (TArrayVariableIdentifier *id = dynamic_cast<TArrayVariableIdentifier*>(identifier)) {
    id->load_addr_to_register(Register::IDR1);         // IDR1 = id.addr ; IDR1 cuz load_expr() might be using IDR
    expr->load_expr();               // ACC = expr.value.value
    this->storei(data->get_register(Register::IDR1));  // p(IDR1) = ACC
  } else {
    expr->load_expr();                   // ACC = expr.value.value
    this->store(identifier->get_addr()); // p(id.addr) = ACC
  }
}

void Code::write(TValue *value) {
  value->load_value(); // ACC = value.value
  this->put();         // PUT
}

void Code::read(TIdentifier *identifier) {
  if (TArrayVariableIdentifier *id = dynamic_cast<TArrayVariableIdentifier*>(identifier)) {
    id->load_addr_to_register(Register::IDR);         // IDR = id.addr
    this->get();                    // ACC = input
    this->storei(data->get_register(Register::IDR));  // p(IDR) = ACC
  } else {
    this->get();                         // ACC = input
    this->store(identifier->get_addr()); // p(id.addr) = ACC
  }
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

void Code::jump() {
  this->atomic("JUMP", -1);
}

void Code::jpos(integer j) {
  this->atomic("JPOS", instruction_count + j);
}

void Code::jpos() {
  this->atomic("JPOS", -1);
}

void Code::jzero(integer j) {
  this->atomic("JZERO", instruction_count + j);
}

void Code::jzero() {
  this->atomic("JZERO", -1);
}

void Code::jneg(integer j) {
  this->atomic("JNEG", instruction_count + j);
}

void Code::jneg() {
  this->atomic("JNEG", -1);
}

void Code::halt() {
  this->atomic("HALT");
}

/*
 *********************
 * memory managmnent *
 *********************
*/

void Code::insert_to_acc(integer value) {
  this->reset_acc();
  if (abs(value) > 10) {
    std::string bin_value;
    unsigned long long x = abs(value); 
    for(; x > 0; x /= 2) bin_value += char(x%2 + '0');
    std::reverse(bin_value.begin(), bin_value.end());
    for (std::string::size_type i = 0; i < bin_value.size() - 1; ++i) {
      if (bin_value[i] == '1') (value > 0) ? inc() : dec();
      lshift();
    }
    if (bin_value[bin_value.size() - 1] == '1') value > 0 ? inc() : dec();
  } else {
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
}

void Code::reset_acc() {
  this->sub(0);
}
