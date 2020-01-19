#include "code.hpp"

#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include <algorithm>

// integer Code::get_instruction_count() {
//   return instruction_count;
// } 

// void Code::insert_jump_address(integer k, integer j) {
//   std::string& jump = code[k];
//   std::size_t pos = jump.find("-1");
//   if (pos != std::string::npos)
//     jump.replace(pos, 2, std::to_string(j));
// }

// void Code::insert_jump_address(integer k) {
//   std::string& jump = code[k];
//     std::size_t pos = jump.find("-1");
//     if (pos != std::string::npos)
//       jump.replace(pos, 2, std::to_string(instruction_count));
// }

// sets shifts
InstructionVector Code::start_code() {
  InstructionVector instructions;
  instructions
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

void Code::fprint_code(std::string filename) {
  std::ofstream fout(filename);
  for (auto &line : this->code) {
    fout << line << std::endl;
  }
  fout.close();
}

// void Code::update_offset(integer value) {
//   this->instruction_count += value;
// }

integer next_power_of_two_exponent(integer n) {
  return ceil(log2(n));
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

InstructionVector Code::set_sign_bit(TIdentifier* lid, TIdentifier* rid, Instruction* next) {
  InstructionVector instructions;
  instructions
  .append(reset_acc())
  .push(new Store(Register::D))
  .append(lid->load());
  
  auto j = new Jpos(); instructions.push(j)
    .append(lid->negate())
    .append(reset_acc())
    .push(new Inc())
    .push(new Store(Register::D));
  
  auto jj = rid->load(); instructions.append(jj);
  j->set_jump_destination(jj.begin()); 
  
  j = new Jpos(); instructions.push(j)
    .append(rid->negate())
    .push(new Load(Register::D));
    auto k = new Jzero(); instructions.push(k)
      .push(new Dec());
      auto l = new Jump(); instructions.push(l);
    auto i = new Inc(); instructions.push(i);
    l->set_jump_destination(i); 
    instructions.push(new Store(Register::D));
  j->set_jump_destination(next);

  return instructions;
}

/*
 ***********
 * atomics *
 ***********
*/

// void Code::atomic(std::string instr) {
//   this->code.push_back(instr);
//   this->update_offset(1);
// }

// void Code::atomic(std::string instr, integer i) {
//   std::string next_instruction = instr + " " + std::to_string(i);
//   if (instr == "LOAD") {
//     if (this->code.back().substr(0, 5) == "STORE" and this->code.back().substr(6) == std::to_string(i)) {
//       // std::cerr << instruction_count << ": " << this->code.back().substr(0, 5) << " " << this->code.back().substr(6) << "; "<< std::to_string(i) << std::endl;
//     } 
//     if (this->code.back().substr(0, 5) == "STORE" and this->code.back().substr(6) == std::to_string(i)) {
//       // return;
//     }
//   }
//   this->code.push_back(next_instruction);
//   this->update_offset(1);
// }

// void Code::get() {
//   this->atomic("GET");
// }

// void Code::put() {
//   this->atomic("PUT");
// }

// void Code::load(integer i) {
//   this->atomic("LOAD", i);
// }

// void Code::store(integer i) {
//   this->atomic("STORE", i);
// }

// void Code::loadi(integer i) {
//   this->atomic("LOADI", i);
// }

// void Code::storei(integer i) {
//   this->atomic("STOREI", i);
// }

// void Code::add(integer i) {
//   this->atomic("ADD", i);
// }

// void Code::sub(integer i) {
//   this->atomic("SUB", i);
// }

// void Code::shift(integer i) {
//   this->atomic("SHIFT", i);
// }

// void Code::inc() {
//   this->atomic("INC");
// }

// void Code::dec() {
//   this->atomic("DEC");
// }

// void Code::jump(integer j) {
//   this->atomic("JUMP", j);
// }

// integer Code::jump() {
//   this->atomic("JUMP", -1);
//   return instruction_count - 1;
// }

// void Code::jpos(integer j) {
//   this->atomic("JPOS", instruction_count + j);
// }

// integer Code::jpos() {
//   this->atomic("JPOS", -1);
//   return instruction_count - 1;
// }

// void Code::jzero(integer j) {
//   this->atomic("JZERO", instruction_count + j);
// }

// integer Code::jzero() {
//   this->atomic("JZERO", -1);
//   return instruction_count - 1;
// }

// void Code::jneg(integer j) {
//   this->atomic("JNEG", instruction_count + j);
// }

// integer Code::jneg() {
//   this->atomic("JNEG", -1);
//   return instruction_count - 1;
// }

// void Code::halt() {
//   this->atomic("HALT");
// }

/*
 *********************
 * memory managmnent *
 *********************
*/

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
