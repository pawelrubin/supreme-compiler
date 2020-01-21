#include "types.hpp"
#include "code.hpp"

integer NumberValue::get_value() {
  return this->value;
}

InstructionVector NumberValue::load_value() {
  InstructionVector instructions;
  instructions.append(code->insert_to_acc(this->value));
  return instructions;
}

InstructionVector NumberValue::store_in_register(Register reg) {
  InstructionVector instructions;
  instructions.append(code->insert_to_acc(this->value))
  .push(new Store(data->get_register(reg)));
  return instructions;
}

IdentifierValue::IdentifierValue(TIdentifier *identifier) {
  this->identifier = identifier;
}

InstructionVector IdentifierValue::load_value() {
  InstructionVector instructions;
  if (TArrayVariableIdentifier *avid = dynamic_cast<TArrayVariableIdentifier*>(this->identifier)) {
    instructions.append(avid->load_addr_to_register(Register::IDR))
    .push(new Loadi(data->get_register(Register::IDR)));
  } else {
    instructions.push(new Load(this->identifier->get_addr()));
  }
  return instructions;
}


InstructionVector IdentifierValue::store_in_register(Register reg) {
  InstructionVector instructions;
  instructions.append(this->identifier->load_value_to_register(reg));
  return instructions;
}

TIdentifier* IdentifierValue::get_identifier() {
  return this->identifier;
}
