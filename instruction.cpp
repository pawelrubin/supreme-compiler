#include "instruction.hpp"

#include <iostream>

Assembler& Assembler::push(Instruction* instruction) {
  this->instructions.push_back(instruction);
  return *this;
}

Assembler& Assembler::append(InstructionList next_instructions) {
  this->instructions.insert(
    std::end(this->instructions),
    std::begin(next_instructions),
    std::end(next_instructions)
  );
  return *this;
}

void Instruction::set_pc(integer pc) {
  this->pc = pc;
}

integer Instruction::get_pc() {
  return this->pc;
}

std::string SimpleInstruction::assembly() {
  return this->code;
}

std::string MemoryInstruction::assembly() {
  return this->code + " " + std::to_string(this->address);
}

std::string JumpInstruction::assembly() {
  return this->code + " " + std::to_string(this->jump_destination->get_pc());
}

void JumpInstruction::set_jump_destination(Instruction* jump_destination) {
  this->jump_destination = jump_destination;
}

// TODO assert that jump destinations are set
codeList Assembler::generateCode() {
  codeList code;
  integer pc = 0;
  for (const auto &i : this->instructions) {
    i->set_pc(pc);
    ++pc;
  }
  for (const auto &i : this->instructions) {
    auto c = i->assembly();
    std::cout<<i->get_pc()<<": "<<c<<std::endl;
    code.push_back(c);
  }
  return code;
}
// void Code::peephole(integer start, integer end) {
//   std::vector<std::string>::iterator it = code.begin() + start;
//   integer i = 0;
//   while (it != code.begin() + end - i - 1) {
//     if ((*it).substr(0, 5) == "STORE") {
//       std::string k = (*it).substr(6);
//       if ((*(it + 1)).substr(0, 4) == "LOAD") {
//         if (k == (*(it + 1)).substr(5)) {
//           if (jumps.count(std::stoll(k)) == 0) {
//             // code.erase(it + 1);
//             std::cerr << "LOAD "<< k << " at line " << distance(code.begin(),it+1) << std::endl;
//             // ++i;
//             // continue;
//           } 
//         }
//       }
//     }
//     ++it;
//   }
// }
void Assembler::peephole() {
  std::vector<Instruction*>::iterator it = this->instructions.begin();
  while (it != this->instructions.end()) {
    if (auto s = dynamic_cast<Store*>(*it)) {
      if (auto l = dynamic_cast<Load*>(*(it + 1))) {
        if (s->get_pc() == l->get_pc()) {
          this->instructions.erase(it + 1);
          continue;
        }
      }
    }
    ++it;
  } 
}