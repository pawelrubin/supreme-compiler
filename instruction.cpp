#include "instruction.hpp"

#include <iostream>

void Instruction::set_unsafe() {
  this->safe_to_delete = false;
}

bool Instruction::is_safe_to_delete() {
  return this->safe_to_delete;
}

MemoryInstruction::MemoryInstruction(Register reg) : Instruction() {
  this->address = data->get_register(reg);
}

integer MemoryInstruction::get_address() {
  return this->address;
}

InstructionVector& InstructionVector::push(Instruction* instruction) {
  this->instructions.push_back(instruction);
  return *this;
}

InstructionVector& InstructionVector::append(InstructionVector next_instructions) {
  this->instructions.insert(
    std::end(this->instructions),
    std::begin(*next_instructions.get_vector()),
    std::end(*next_instructions.get_vector())
  );
  return *this;
}

std::vector<Instruction*>* InstructionVector::get_vector() {
  return &(this->instructions);
}

Instruction* InstructionVector::begin() {
  return *this->instructions.begin();
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
  jump_destination->set_unsafe();
  this->jump_destination = jump_destination;
  std::cerr << "SETTING DESTINATION: "<<this->jump_destination->get_code()<<this->jump_destination<<std::endl;

}

// TODO assert that jump destinations are set
codeList CodeGenerator::generateCode() {
  codeList code;
  integer pc = 0;

  for (const auto &i : *this->instructions.get_vector()) {
    if (not dynamic_cast<NOP*>(i)) {
      i->set_pc(pc);
      ++pc;
    }
  }

  auto vector = this->instructions.get_vector();
  auto it = vector->begin();

  while (it + 1 != vector->end()) {
    if (auto nop = dynamic_cast<NOP*>(*it)) {
      for (const auto &j : nop->jumpers) {
        int i = 1;
        do {
          j->set_jump_destination(*(it + i));
          ++i;
        } while (dynamic_cast<NOP*>(*(it + i)));
        std::cout<<(*(it + 1))->assembly()<<std::endl;
      }
    } else {
      auto c = (*it)->assembly();
      std::cout<<(*it)->get_pc()<<": "<<c<< " "<<(*it)<< std::endl;
      code.push_back(c);
    }
    ++it;
  }

  // for (const auto &i : *this->instructions.get_vector()) {
  //   if (auto nop = dynamic_cast<NOP*>(i)) {
  //     for (const auto j : nop->jumpers) {
  //       j->set_jump_destination(i+1);
  //       std::cout<<(i+1)->assembly()<<std::endl;
  //     }
  //   } else {
  //     auto c = i->assembly();
  //     std::cout<<i->get_pc()<<": "<<c<<std::endl;
  //     code.push_back(c);
  //   }
  // }
  return code;
}

void CodeGenerator::peephole() {
  auto vector = this->instructions.get_vector();
  auto it = vector->begin();
  while (it + 1 != vector->end()) {
    if (auto store = dynamic_cast<Store*>(*it)) {
      if (auto load = dynamic_cast<Load*>(*(it + 1))) {
        if (store->get_address() == load->get_address()) {
          if (load->is_safe_to_delete()) {
            vector->erase(it + 1);
            continue;
          }
        }
      }
    }
    ++it;
  } 
}

void InstructionVectorWithJump::set_jump(JumpInstruction* jump) {
  this->jump = jump;
}

JumpInstruction* InstructionVectorWithJump::get_jump() {
  return this->jump;
}

NOP::NOP(JumpInstruction* jump) {
  this->jumpers.push_back(jump);
}

std::string Instruction::get_code() {
  return this->code;
}