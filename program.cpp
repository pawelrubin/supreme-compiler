#include "types.hpp"
#include "code.hpp"

#include <iostream>

InstructionVector TProgram::load_program() {
  InstructionVector instructions;
  instructions
  .append(code->start_code())
  .append(commands->load_commands())
  .append(code->end_code());
  return instructions;
}

// TDeclaration::TDeclaration(ident pidentifier) {
//   this->pidentifier = pidentifier;
// }

// void TDeclaration::load_declaration() {
//   data->declare_variable(this->pidentifier);
// }

// TArrayDeclaration::TArrayDeclaration(ident pidentifier, integer start, integer end) : TDeclaration(pidentifier) {
//   this->start = start;
//   this->end = end;
// }

// void TArrayDeclaration::load_declaration() {
//   data->declare_array(this->pidentifier, this->start, this->end);
// }
