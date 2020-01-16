#include "types.hpp"
#include "code.hpp"


void TIdentifier::load_value_to_register(Register reg) {
  this->load_value_to_acc();
  code->store(data->get_register(reg));
}


TVariableIdentifier::TVariableIdentifier(ident var_name) : TIdentifier(var_name) {
  if (data->is_declared(var_name)) {
    this->variable = static_cast<Variable *>(data->get_symbol(var_name));
  } else {
    this->variable = data->declare_bad_variable(var_name);
  }
}

TVariableIdentifier::TVariableIdentifier(Variable *pseudo) {
  this->variable = pseudo;
}

void TVariableIdentifier::load_addr_to_register(Register reg) {
  code->insert_to_acc(this->variable->get_addr());
  code->store(data->get_register(reg));
}

integer TVariableIdentifier::get_addr() {
  return this->variable->get_addr();
}

void TVariableIdentifier::load_value_to_acc() {
  code->load(this->variable->get_addr());
}

void TVariableIdentifier::negate(bool is_loaded) {
  code->reset_acc();
  code->sub(this->get_addr());
  code->store(this->get_addr());
}


TArrayVariableIdentifier::TArrayVariableIdentifier(ident arr_name, ident var_name) : TIdentifier(arr_name + var_name) {
  this->array = static_cast<Array *>(data->get_symbol(arr_name));
  if (data->is_declared(var_name)) {
    this->variable = static_cast<Variable *>(data->get_symbol(var_name));
  } else {
    this->variable = data->declare_bad_variable(var_name);
  }
}

void TArrayVariableIdentifier::load_addr_to_register(Register reg) {
  code->insert_to_acc(this->array->get_norm_addr());
  code->add(this->variable->get_addr());
  code->store(data->get_register(reg));
}

void TArrayVariableIdentifier::load_value_to_acc() {
  code->insert_to_acc(this->array->get_norm_addr());
  code->add(this->variable->get_addr());
  code->loadi(0);
}

void TArrayVariableIdentifier::negate(bool is_loaded) {
  if (!is_loaded) {
    code->insert_to_acc(this->array->get_norm_addr());
    code->add(this->variable->get_addr());
    code->store(data->get_register(Register::IDR));
  }
  code->reset_acc();
  code->sub(data->get_register(Register::IDR));
  code->store(data->get_register(Register::IDR));
}


TArrayNumIdentifier::TArrayNumIdentifier(ident arr_name, integer num_value) : TIdentifier(arr_name) {
  this->array = static_cast<Array *>(data->get_symbol(arr_name));
  this->num_value = num_value;
}

void TArrayNumIdentifier::load_addr_to_register(Register reg) {
  code->insert_to_acc(this->array->get_addr(this->num_value));
  code->store(data->get_register(reg));
}

integer TArrayNumIdentifier::get_addr() {
  return this->array->get_addr(this->num_value);
}

void TArrayNumIdentifier::load_value_to_acc() {
  code->load(this->array->get_addr(this->num_value));
}

void TArrayNumIdentifier::negate(bool is_loaded) {
  code->reset_acc();
  code->sub(this->get_addr());
  code->store(this->get_addr());
}
