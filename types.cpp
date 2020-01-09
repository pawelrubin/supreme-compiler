#include "types.hpp"

integer TIdentifier::get_mem_addr() {
  return this->mem_addr;
}

TVariableIdentifier::TVariableIdentifier(ident var_name) {
  this->mem_addr = data->get_symbol(var_name)->get_addr();
}

TArrayVariableIdentifier::TArrayVariableIdentifier(ident arr_name, ident var_name) {
  this->mem_addr = 0;
}

TArrayNumIdentifier::TArrayNumIdentifier(ident arr_name, integer num_value) {
  this->mem_addr = ((Array *)(data->get_symbol(arr_name)) )->get_addr(num_value);
}

NumberValue::NumberValue(integer value) {
  this->value = value;
}

integer TValue::get_value() {
  return this->value;
}

IdentifierValue::IdentifierValue(TIdentifier *identifier) {
  this->identifier = identifier;
}
