#include "types.hpp"
#include "code.hpp"

#include <iostream>

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
    break;

  case BinaryOperator::TIMES:
    this->times();
    
    break;
  
  case BinaryOperator::DIV:
    this->div();
    
    break;
  
  case BinaryOperator::MOD:
    this->mod();
    
    break;
  }
}

void TBinaryExpression::plus() {
  if (NumberValue *lv = dynamic_cast<NumberValue*>(lvalue)) {
    if (NumberValue *rv = dynamic_cast<NumberValue*>(rvalue)) {
      code->insert_to_acc(lv->get_value() + rv->get_value());
    } else {
      TIdentifier *id = static_cast<IdentifierValue*>(rvalue)->get_identifier();
      if (TArrayVariableIdentifier *avid = dynamic_cast<TArrayVariableIdentifier*>(id)) {
        avid->load_addr_to_idr();
        code->loadi(data->get_IDR());
        code->store(data->get_IDR());
        code->insert_to_acc(lv->get_value());
        code->add(data->get_IDR());
      } else {
        code->insert_to_acc(lv->get_value()); 
        code->add(id->get_addr());
      }
    }
  } else {
    if (NumberValue *rv = dynamic_cast<NumberValue*>(rvalue)) {
      TIdentifier *id = static_cast<IdentifierValue*>(lvalue)->get_identifier();
      if (TArrayVariableIdentifier *avid = dynamic_cast<TArrayVariableIdentifier*>(id)) {
        avid->load_addr_to_idr();
        code->loadi(data->get_IDR());
        code->store(data->get_IDR());
        code->insert_to_acc(rv->get_value());
        code->add(data->get_IDR());
      } else {
        code->insert_to_acc(rv->get_value()); 
        code->add(id->get_addr());
      }
    } else {
      TIdentifier *lid = static_cast<IdentifierValue*>(lvalue)->get_identifier();
      TIdentifier *rid = static_cast<IdentifierValue*>(rvalue)->get_identifier();

      if (TArrayVariableIdentifier *lavid = dynamic_cast<TArrayVariableIdentifier*>(lid)) {
        if (TArrayVariableIdentifier *ravid = dynamic_cast<TArrayVariableIdentifier*>(rid)) {      
          lavid->load_addr_to_idr();     // IDR = &lval
          ravid->load_addr_to_idr(1);    // IDR1= &rval 
          code->loadi(data->get_IDR());  // ACC = lval
          code->store(data->get_IDR());  // IDR = lval
          code->loadi(data->get_IDR(1)); // ACC = rval
          code->add(data->get_IDR());    // ACC = rval + IDR = rval + lval
        } else {
          lavid->load_addr_to_idr();
          code->loadi(data->get_IDR());          
          code->add(rid->get_addr());
        }
      } else {
        if (TArrayVariableIdentifier *ravid = dynamic_cast<TArrayVariableIdentifier*>(rid)) {
          ravid->load_addr_to_idr();
          code->loadi(data->get_IDR());          
          code->add(rid->get_addr());
        } else { 
          code->load(rid->get_addr());
          code->add(lid->get_addr());
        }
      }
    }
  }
}


/* TODO: Substraction */
void TBinaryExpression::minus() {
  if (NumberValue *lv = dynamic_cast<NumberValue*>(lvalue)) {
    if (NumberValue *rv = dynamic_cast<NumberValue*>(rvalue)) {
      code->insert_to_acc(lv->get_value() - rv->get_value());
    }else {
      TIdentifier *id = static_cast<IdentifierValue*>(rvalue)->get_identifier();
      if (TArrayVariableIdentifier *avid = dynamic_cast<TArrayVariableIdentifier*>(id)) {
        avid->load_addr_to_idr();
        code->loadi(data->get_IDR());
        code->store(data->get_IDR());
        code->insert_to_acc(lv->get_value());
        code->add(data->get_IDR());
      } else {
        code->insert_to_acc(lv->get_value()); 
        code->add(id->get_addr());
      }
    }
  } else {
    if (NumberValue *rv = dynamic_cast<NumberValue*>(rvalue)) {
      TIdentifier *id = static_cast<IdentifierValue*>(lvalue)->get_identifier();
      if (TArrayVariableIdentifier *avid = dynamic_cast<TArrayVariableIdentifier*>(id)) {
        avid->load_addr_to_idr();
        code->loadi(data->get_IDR());
        code->store(data->get_IDR());
        code->insert_to_acc(rv->get_value());
        code->sub(data->get_IDR());
      } else {
        code->insert_to_acc(rv->get_value()); 
        code->sub(id->get_addr());
      }
    } else {
      TIdentifier *lid = static_cast<IdentifierValue*>(lvalue)->get_identifier();
      TIdentifier *rid = static_cast<IdentifierValue*>(rvalue)->get_identifier();

      if (TArrayVariableIdentifier *lavid = dynamic_cast<TArrayVariableIdentifier*>(lid)) {
        if (TArrayVariableIdentifier *ravid = dynamic_cast<TArrayVariableIdentifier*>(rid)) {      
          lavid->load_addr_to_idr();     // IDR = &lval
          ravid->load_addr_to_idr(1);    // IDR1= &rval 
          code->loadi(data->get_IDR(1));  // ACC = rval
          code->store(data->get_IDR(1));  // IDR1 = rval
          code->loadi(data->get_IDR()); // ACC = lval
          code->sub(data->get_IDR(1));    // ACC = lval - IDR1 = lval - rval
        } else {
          lavid->load_addr_to_idr();
          code->loadi(data->get_IDR());          
          code->sub(rid->get_addr());
        }
      } else {
        if (TArrayVariableIdentifier *ravid = dynamic_cast<TArrayVariableIdentifier*>(rid)) {
          ravid->load_addr_to_idr();
          code->loadi(data->get_IDR());          
          code->sub(rid->get_addr());
        } else { 
          code->load(rid->get_addr());
          code->sub(lid->get_addr());
        }
      }
    }
  }
}

/* TODO: Multiplication */
void TBinaryExpression::times() {
  if (NumberValue *lv = dynamic_cast<NumberValue*>(lvalue)) {
    if (NumberValue *rv = dynamic_cast<NumberValue*>(rvalue)) {
      code->insert_to_acc(lv->get_value() * rv->get_value());
    }
  }
}

/* TODO: Division */
void TBinaryExpression::div() {
  if (NumberValue *lv = dynamic_cast<NumberValue*>(lvalue)) {
    if (NumberValue *rv = dynamic_cast<NumberValue*>(rvalue)) {
      if (rv->get_value() == 0) code->insert_to_acc(0);
      else code->insert_to_acc(lv->get_value() / rv->get_value());
    }
  }
}

/* TODO: Modulo */
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
  if (TArrayVariableIdentifier *avid = dynamic_cast<TArrayVariableIdentifier*>(this->identifier)) {
    avid->load_addr_to_idr();
    code->loadi(data->get_IDR());
  } else {
    code->load(this->identifier->get_addr());
  }
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

void TVariableIdentifier::load_addr_to_idr(int id /*=0*/) {
  code->insert_to_acc(this->variable->get_addr());
  code->store(data->get_IDR(id));
}

integer TVariableIdentifier::get_addr() {
  return this->variable->get_addr();
}


TArrayVariableIdentifier::TArrayVariableIdentifier(ident arr_name, ident var_name) {
  this->array = static_cast<Array *>(data->get_symbol(arr_name));
  this->variable = static_cast<Variable *>(data->get_symbol(var_name));
}

void TArrayVariableIdentifier::load_addr_to_idr(int id /*=0*/) {
  code->insert_to_acc(this->array->get_norm_addr());
  code->add(this->variable->get_addr());
  code->store(data->get_IDR(id));
}


TArrayNumIdentifier::TArrayNumIdentifier(ident arr_name, integer num_value) {
  this->array = static_cast<Array *>(data->get_symbol(arr_name));
  this->num_value = num_value;
}

void TArrayNumIdentifier::load_addr_to_idr(int id /*=0*/) {
  code->insert_to_acc(this->array->get_addr(this->num_value));
  code->store(data->get_IDR(id));
}

integer TArrayNumIdentifier::get_addr() {
  return this->array->get_addr(this->num_value);
}

