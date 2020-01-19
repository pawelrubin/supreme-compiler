#include "types.hpp"
#include "code.hpp"

integer NumberValue::get_value() {
  return this->value;
}

InstructionVector NumberValue::load_value() {
  code->insert_to_acc(this->value);
}

InstructionVector NumberValue::insert_to_VLR() {
  code->insert_to_acc(this->value);
  code->store(data->get_register(Register::VLR));
}

InstructionVector NumberValue::store_in_register(Register reg) {
  code->insert_to_acc(this->value);
  code->store(data->get_register(reg));
}

IdentifierValue::IdentifierValue(TIdentifier *identifier) {
  this->identifier = identifier;
}

InstructionVector IdentifierValue::load_value() {
  if (TArrayVariableIdentifier *avid = dynamic_cast<TArrayVariableIdentifier*>(this->identifier)) {
    avid->load_addr_to_register(Register::IDR);
    code->loadi(data->get_register(Register::IDR));
  } else {
    code->load(this->identifier->get_addr());
  }
}

InstructionVector IdentifierValue::insert_to_VLR() {
  // this->identifier->
}

InstructionVector IdentifierValue::store_in_register(Register reg) {
  // code->insert_to_acc(this->value);
  // code->store(data->get_register(reg));
  this->identifier->load_value_to_register(reg);
}

TIdentifier* IdentifierValue::get_identifier() {
  return this->identifier;
}
