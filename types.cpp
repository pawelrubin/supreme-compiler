#include "types.hpp"
#include "code.hpp"

integer TIdentifier::get_mem_addr() {
  return this->mem_addr;
}

//

TVariableIdentifier::TVariableIdentifier(ident var_name) {
  this->variable = static_cast<Variable *>(data->get_symbol(var_name));
  this->mem_addr = this->variable->get_addr();
}

void TVariableIdentifier::load_identifier_to_reg() {
  code->insert_to_acc(this->variable->get_addr());
  code->store(data->get_id_reg_addr());
}

//

TArrayVariableIdentifier::TArrayVariableIdentifier(ident arr_name, ident var_name) {
  this->mem_addr = -1;
}

// void TArrayVariableIdentifier::load_identifier_to_reg() {
//   // code->insert_to_acc(this->variable->get_addr());
//   // code->store(data->get_id_reg_addr());
// }


TArrayNumIdentifier::TArrayNumIdentifier(ident arr_name, integer num_value) {
  this->mem_addr = (static_cast<Array *>(data->get_symbol(arr_name)))->get_addr(num_value);
}

// void TArrayNumIdentifier::load_identifier_to_reg() {
//   // code->insert_to_acc(this->variable->get_addr());
//   // code->store(data->get_id_reg_addr());
// }

NumberValue::NumberValue(integer value) {
  this->value = value;
}

integer TValue::get_value() {
  return this->value;
}

IdentifierValue::IdentifierValue(TIdentifier *identifier) {
  this->identifier = identifier;
}


// Expression

TValueExpression::TValueExpression(TValue *value) {
  this->value = value;
}

void TValueExpression::load_expr() {
  code->insert_to_acc(this->value->get_value());
}