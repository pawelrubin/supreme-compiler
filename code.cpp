#include "code.hpp"

#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <algorithm>

// sets shifts
InstructionVector Code::start_code() {
  InstructionVector instructions;
  instructions
  .push(new Sub(0))
  .push(new Dec())
  .push(new Store(2))
  .push(new Inc())
  .push(new Inc())
  .push(new Store(1));
  return instructions;
}

InstructionVector Code::end_code() {
  InstructionVector instructions;
  instructions.push(new Halt());
  return instructions;
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

void Code::fprint_code(std::string filename, codeList code) {
  std::ofstream fout(filename);
  for (auto &line : code) {
    fout << line << std::endl;
  }
  fout.close();
}

/*
 *************************
 * high order operations *
 *************************
*/

InstructionVector Code::parity_test(TIdentifier* id) {
  InstructionVector instructions;
  if (TArrayVariableIdentifier *avid = dynamic_cast<TArrayVariableIdentifier*>(id)) {
    instructions.append(avid->load_value_to_register(Register::IDR));
    id = new TVariableIdentifier(new Variable(data->get_register(Register::IDR)));
  }
  instructions
  .append(id->load())
  .append(rshift())
  .append(lshift())
  .push(new Sub(id->get_addr()));

  return instructions;
}

InstructionVector Code::lshift() {
  InstructionVector instructions;
  instructions.push(new Shift(1));
  return instructions;
}

InstructionVector Code::rshift() {
  InstructionVector instructions;
  instructions.push(new Shift(2));
  return instructions;
}

InstructionVector Code::set_sign_bit(TIdentifier* lid, TIdentifier* rid) {
  InstructionVector instructions;
  instructions
  .push(new Sub(0))
  .push(new Store(Register::D))
  .append(lid->load());
  
  auto j = new Jpos(); instructions.push(j)
    .append(lid->negate())
    .push(new Sub(0))
    .push(new Inc())
    .push(new Store(Register::D))
  .push(new NOP(j))
  
  .append(rid->load());
  j = new Jpos(); 
    instructions.push(j)
    .append(rid->negate())
    .push(new Load(Register::D));
    auto k = new Jzero();
      instructions.push(k)
      .push(new Dec());
      auto l = new Jump();
      instructions.push(l)
    .push(new NOP(k))
    .push(new Inc())
    .push(new NOP(l))
    .push(new Store(Register::D))
  .push(new NOP(j));

  return instructions;
}

InstructionVector Code::insert_to_acc(integer value) {
  InstructionVector instructions;
  instructions.push(new Sub(0));
  if (llabs(value) > 10) {
    std::string bin_value;
    long long x = llabs(value); 
    for(; x > 0; x /= 2) bin_value += char(x%2 + '0');
    std::reverse(bin_value.begin(), bin_value.end());
    for (std::string::size_type i = 0; i < bin_value.size() - 1; ++i) {
      if (bin_value[i] == '1') {
        if (value > 0) {
          instructions.push(new Inc()); 
        } else {
          instructions.push(new Dec());
        }
      }
      instructions.append(lshift());
    }
    if (bin_value[bin_value.size() - 1] == '1') {
      if (value > 0) {
        instructions.push(new Inc());
      } else {
        instructions.push(new Dec());
      }
    }
  } else {
    if (value > 0) {
      for (int i = 0; i < value; i++) {
        instructions.push(new Inc());
      }
    } else {
      for (int i = value; i < 0; i++) {
        instructions.push(new Dec());
      }
    }
  }
  return instructions;
}

bool Code::is_power_of_two(integer n) {
  return (n & (n - 1)) == 0;
}
