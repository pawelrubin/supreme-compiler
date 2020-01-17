#include "instruction.hpp"

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
  return this->code + " " + std::to_string(this->destination->get_pc());
}
