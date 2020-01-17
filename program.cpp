#include "types.hpp"
#include "code.hpp"

#include <iostream>

void TProgram::load_program() {
  code->start_code();
  commands->load_commands();
  code->end_code();
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
