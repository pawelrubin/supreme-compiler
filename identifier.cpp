#include "types.hpp"
#include "code.hpp"


InstructionVector TIdentifier::load_value_to_register(Register reg) {
  InstructionVector instructions;
  instructions.append(this->load())
  .push(new Store(data->get_register(reg)));
  return instructions;
}


TVariableIdentifier::TVariableIdentifier(ident var_name) : TIdentifier(var_name) {
  if (data->is_declared(var_name)) {
    this->variable = static_cast<Variable *>(data->get_symbol(var_name));
  } else {
    this->variable = data->declare_bad_variable(var_name);
  }
}

InstructionVector TVariableIdentifier::load_addr_to_register(Register reg) {
  InstructionVector instructions;
  instructions.append(code->insert_to_acc(this->variable->get_addr()))
  .push(new Store(data->get_register(reg)));
  return instructions;
}

integer TVariableIdentifier::get_addr() {
  return this->variable->get_addr();
}

InstructionVector TVariableIdentifier::load() {
  InstructionVector instructions;
  instructions.push(new Load(this->variable->get_addr()));
  return instructions;
}

InstructionVector TVariableIdentifier::negate() {
  InstructionVector instructions;
  instructions.push(new Sub(0))
  .push(new Sub(this->get_addr()))
  .push(new Store(this->get_addr()));
  return instructions;
}


TArrayVariableIdentifier::TArrayVariableIdentifier(ident arr_name, ident var_name) : TIdentifier(arr_name + var_name) {
  this->array = static_cast<Array *>(data->get_symbol(arr_name));
  if (data->is_declared(var_name)) {
    this->variable = static_cast<Variable *>(data->get_symbol(var_name));
  } else {
    this->variable = data->declare_bad_variable(var_name);
  }
}

InstructionVector TArrayVariableIdentifier::load_addr_to_register(Register reg) {
  InstructionVector instructions;
  instructions.append(code->insert_to_acc(this->array->get_norm_addr()))
  .push(new Add(this->variable->get_addr()))
  .push(new Store(data->get_register(reg)));
  return instructions;
}

InstructionVector TArrayVariableIdentifier::load() {
  InstructionVector instructions;
  instructions.append(code->insert_to_acc(this->array->get_norm_addr()))
  .push(new Add(this->variable->get_addr()))
  .push(new Loadi(0));
  return instructions;
}

// negates tab(a), assuming that tab(a) address is in IDR register
InstructionVector TArrayVariableIdentifier::negate() {
  InstructionVector instructions;
  instructions
  .push(new Sub(0))
  .push(new Sub(data->get_register(Register::IDR)))
  .push(new Store(data->get_register(Register::IDR)));
  return instructions;
}

TArrayNumIdentifier::TArrayNumIdentifier(ident arr_name, integer num_value) : TIdentifier(arr_name) {
  this->array = static_cast<Array *>(data->get_symbol(arr_name));
  this->num_value = num_value;
}

InstructionVector TArrayNumIdentifier::load_addr_to_register(Register reg) {
  InstructionVector instructions;
  instructions
  .append(code->insert_to_acc(this->array->get_addr(this->num_value)))
  .push(new Store(data->get_register(reg)));
  return instructions;
}

integer TArrayNumIdentifier::get_addr() {
  return this->array->get_addr(this->num_value);
}

InstructionVector TArrayNumIdentifier::load() {
  InstructionVector instructions;
  instructions
  .push(new Load(this->array->get_addr(this->num_value)));
  return instructions;
}

InstructionVector TArrayNumIdentifier::negate() {
  InstructionVector instructions;
  instructions
  .push(new Sub(0))
  .push(new Sub(this->get_addr()))
  .push(new Store(this->get_addr()));
  return instructions;
}

integer TArrayNumIdentifier::get_num() {
  return this->num_value;
}
