#include "types.hpp"
#include "code.hpp"

/*
 ********************
 *    EXPRESSION    *
 ********************
 */

TValueExpression::TValueExpression(TValue *value) {
  this->value = value;
}

void TValueExpression::load_expr() {
  code->insert_to_acc(this->value->get_value());
}


/*
 ***************
 *    VALUE    *
 ***************
 */

NumberValue::NumberValue(integer value) {
  this->value = value;
}

integer TValue::get_value() {
  return this->value;
}

void NumberValue::load_value() {
  code->insert_to_acc(this->value);
}

IdentifierValue::IdentifierValue(TIdentifier *identifier) {
  this->identifier = identifier;
}

void IdentifierValue::load_value() {
  this->identifier->load_addr_to_idr();
  code->loadi(data->get_IDR());
}


/*
 ********************
 *    IDENTIFIER    *
 ********************
 */

TVariableIdentifier::TVariableIdentifier(ident var_name) {
  this->variable = static_cast<Variable *>(data->get_symbol(var_name));
}

void TVariableIdentifier::load_addr_to_idr() {
  code->insert_to_acc(this->variable->get_addr());
  code->store(data->get_IDR());
}

TArrayVariableIdentifier::TArrayVariableIdentifier(ident arr_name, ident var_name) {
  this->array = static_cast<Array *>(data->get_symbol(arr_name));
  this->variable = static_cast<Variable *>(data->get_symbol(var_name));
}

void TArrayVariableIdentifier::load_addr_to_idr() {
  code->insert_to_acc(this->array->get_norm_addr());
  code->add(this->variable->get_addr());
  code->store(data->get_IDR());
}

TArrayNumIdentifier::TArrayNumIdentifier(ident arr_name, integer num_value) {
  this->array = static_cast<Array *>(data->get_symbol(arr_name));
  this->num_value = num_value;
}

void TArrayNumIdentifier::load_addr_to_idr() {
  code->insert_to_acc(this->array->get_addr(this->num_value));
  code->store(data->get_IDR());
}
