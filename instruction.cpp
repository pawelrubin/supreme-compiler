#include "instruction.hpp"
#include "exceptions.hpp"

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
  std::cerr << this->code << " " << this << " : " << "SETTING DESTINATION: "<<this->jump_destination->get_code()<<" "<<this->jump_destination<<std::endl;
}

codeList CodeGenerator::generateCode() {
  codeList code;
  integer pc = 0;

  auto vector = this->instructions.get_vector();
  auto it = vector->begin();

  // setting jump destinations and program counter
  while (it != vector->end()) {
    if (auto nop = dynamic_cast<NOP*>(*it)) {
      if (it + 1 == vector->end()) {
        throw NOPAsLastInstructionException();
      }
      for (const auto &j : nop->jumpers) {
        int i = 1;
        while (dynamic_cast<NOP*>(*(it + i))) {
          ++i;
        }
        j->set_jump_destination(*(it + i));
      }
    } else {
      (*it)->set_pc(pc);
      ++pc;
    }
    ++it;
  }

  vector = this->instructions.get_vector();
  it = vector->begin();

  while (it != vector->end()) {
    if (not dynamic_cast<NOP*>(*it)) {
      code.push_back(std::to_string((*it)->get_pc()) + ": " + (*it)->assembly());
    }
    ++it;
  }
  
  return code;
}

void CodeGenerator::peephole() {
  auto vector = this->instructions.get_vector();
  auto it = vector->begin();
  while (it + 1 !=   vector->end()) {
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