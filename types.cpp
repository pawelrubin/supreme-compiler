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

TBinaryExpression::TBinaryExpression(TValue *lvalue, TValue *rvalue, BinaryOperator op) {
  this->lvalue = lvalue;
  this->rvalue = rvalue;
  this->op = op;
}

void TBinaryExpression::load_expr() {
  switch (this->op) {
  case BinaryOperator::PLUS:
    this->plus();
    break;
  
  case BinaryOperator::MINUS:
    this->minus();
    /* TODO: Substraction */
    break;

  case BinaryOperator::TIMES:
    this->times();
    /* TODO: Multiplication */
    break;
  
  case BinaryOperator::DIV:
    this->div();
    /* TODO: Division */
    break;
  
  case BinaryOperator::MOD:
    this->mod();
    /* TODO: Modulo */
    break;
  }
}

void TBinaryExpression::plus() {
  if (NumberValue *lv = dynamic_cast<NumberValue*>(lvalue)) {
    if (NumberValue *rv = dynamic_cast<NumberValue*>(rvalue)) {
      code->insert_to_acc(lv->get_value() + rv->get_value());
    } else {
      (static_cast<IdentifierValue*>(rvalue))->get_identifier()->load_addr_to_idr(); 
      code->insert_to_acc(lv->get_value()); 
      code->add(data->get_IDR());
    }
  }
}


void TBinaryExpression::minus() {
  if (NumberValue *lv = dynamic_cast<NumberValue*>(lvalue)) {
    if (NumberValue *rv = dynamic_cast<NumberValue*>(rvalue)) {
      code->insert_to_acc(lv->get_value() - rv->get_value());
    }
  }
}

void TBinaryExpression::times() {
  if (NumberValue *lv = dynamic_cast<NumberValue*>(lvalue)) {
    if (NumberValue *rv = dynamic_cast<NumberValue*>(rvalue)) {
      code->insert_to_acc(lv->get_value() * rv->get_value());
    }
  }
}

void TBinaryExpression::div() {
  if (NumberValue *lv = dynamic_cast<NumberValue*>(lvalue)) {
    if (NumberValue *rv = dynamic_cast<NumberValue*>(rvalue)) {
      if (rv->get_value() == 0) code->insert_to_acc(0);
      else code->insert_to_acc(lv->get_value() / rv->get_value());
    }
  }
}

void TBinaryExpression::mod() {
  if (NumberValue *lv = dynamic_cast<NumberValue*>(lvalue)) {
    if (NumberValue *rv = dynamic_cast<NumberValue*>(rvalue)) {
      if (rv->get_value() == 0) code->insert_to_acc(0);
      else code->insert_to_acc((lv->get_value() % rv->get_value() + rv->get_value()) % rv->get_value());
    }
  }
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

TIdentifier* IdentifierValue::get_identifier() {
  return this->identifier;
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

integer TVariableIdentifier::get_addr() {
  return this->variable->get_addr();
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

integer TArrayNumIdentifier::get_addr() {
  return this->array->get_addr(this->num_value);
}

