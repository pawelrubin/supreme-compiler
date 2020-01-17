#include "types.hpp"
#include "code.hpp"

integer NumberValue::get_value() {
  return this->value;
}

void NumberValue::load_value() {
  code->insert_to_acc(this->value);
}

void NumberValue::insert_to_VLR() {
  code->insert_to_acc(this->value);
  code->store(data->get_register(Register::VLR));
}

void NumberValue::store_in_register(Register reg) {
  code->insert_to_acc(this->value);
  code->store(data->get_register(reg));
}

IdentifierValue::IdentifierValue(TIdentifier *identifier) {
  this->identifier = identifier;
}

void IdentifierValue::load_value() {
  if (TArrayVariableIdentifier *avid = dynamic_cast<TArrayVariableIdentifier*>(this->identifier)) {
    avid->load_addr_to_register(Register::IDR);
    code->loadi(data->get_register(Register::IDR));
  } else {
    code->load(this->identifier->get_addr());
  }
}

void IdentifierValue::insert_to_VLR() {
  // this->identifier->
}

void IdentifierValue::store_in_register(Register reg) {
  // code->insert_to_acc(this->value);
  // code->store(data->get_register(reg));
  this->identifier->load_value_to_register(reg);
}

TIdentifier* IdentifierValue::get_identifier() {
  return this->identifier;
}
