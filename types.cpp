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
      TIdentifier *rid = static_cast<IdentifierValue*>(rvalue)->get_identifier();
      if (TArrayVariableIdentifier *avid = dynamic_cast<TArrayVariableIdentifier*>(rid)) {
        avid->load_value_to_idr();            // IDR = rval
        code->insert_to_acc(lv->get_value()); // ACC = lval
        code->add(data->get_IDR());           // ACC = lval + rval
      } else {
        code->insert_to_acc(lv->get_value()); // ACC = lval
        code->add(rid->get_addr());           // ACC = lval + rval
      }
    }
  } else {
    if (NumberValue *rv = dynamic_cast<NumberValue*>(rvalue)) {
      TIdentifier *lid = static_cast<IdentifierValue*>(lvalue)->get_identifier();
      if (TArrayVariableIdentifier *avid = dynamic_cast<TArrayVariableIdentifier*>(lid)) {
        rv->insert_to_VLR();                  // VLR = rval
        avid->load_value_to_acc();            // ACC = lval
        code->add(data->get_VLR());           // ACC = lval + rval           
      } else {
        code->insert_to_acc(rv->get_value()); // ACC = rval
        code->add(lid->get_addr());           // ACC = rval + lval
      }
    } else {
      TIdentifier *lid = static_cast<IdentifierValue*>(lvalue)->get_identifier();
      TIdentifier *rid = static_cast<IdentifierValue*>(rvalue)->get_identifier();

      if (TArrayVariableIdentifier *lavid = dynamic_cast<TArrayVariableIdentifier*>(lid)) {
        if (TArrayVariableIdentifier *ravid = dynamic_cast<TArrayVariableIdentifier*>(rid)) {      
          ravid->load_value_to_idr(); // IDR = rval
          lavid->load_value_to_acc(); // ACC = lval
          code->add(data->get_IDR()); // ACC = lval + IDR = lval + rval
        } else {
          lavid->load_value_to_acc(); // ACC = lval
          code->add(rid->get_addr()); // ACC = lval + rval
        }
      } else {
        if (TArrayVariableIdentifier *ravid = dynamic_cast<TArrayVariableIdentifier*>(rid)) {
          ravid->load_value_to_acc(); // ACC = rval
          code->add(lid->get_addr()); // ACC = rval + lval
        } else { 
          code->load(lid->get_addr()); // ACC = lval 
          code->add(rid->get_addr());  // ACC = lval + rval
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
    }  else {
      TIdentifier *rid = static_cast<IdentifierValue*>(rvalue)->get_identifier();
      if (TArrayVariableIdentifier *avid = dynamic_cast<TArrayVariableIdentifier*>(rid)) {
        avid->load_value_to_idr();            // IDR = rval
        code->insert_to_acc(lv->get_value()); // ACC = lval
        code->sub(data->get_IDR());           // ACC = lval - rval
      } else {
        code->insert_to_acc(lv->get_value()); // ACC = lval
        code->sub(rid->get_addr());           // ACC = lval - rval
      }
    }
  } else {
    if (NumberValue *rv = dynamic_cast<NumberValue*>(rvalue)) {
      TIdentifier *lid = static_cast<IdentifierValue*>(lvalue)->get_identifier();
      if (TArrayVariableIdentifier *avid = dynamic_cast<TArrayVariableIdentifier*>(lid)) {
        rv->insert_to_VLR();                  // VLR = rval
        avid->load_value_to_acc();            // ACC = lval
        code->sub(data->get_VLR());           // ACC = lval - rval           
      } else {
        code->insert_to_acc(rv->get_value()); // ACC = rval
        code->store(data->get_VLR());         // VLR = rval
        lid->load_value_to_acc();             // ACC = lval
        code->sub(data->get_VLR());           // ACC = lval - rval
      }
    } else {
      TIdentifier *lid = static_cast<IdentifierValue*>(lvalue)->get_identifier();
      TIdentifier *rid = static_cast<IdentifierValue*>(rvalue)->get_identifier();

      if (TArrayVariableIdentifier *lavid = dynamic_cast<TArrayVariableIdentifier*>(lid)) {
        if (TArrayVariableIdentifier *ravid = dynamic_cast<TArrayVariableIdentifier*>(rid)) {      
          ravid->load_value_to_idr();    // IDR = rval
          lavid->load_value_to_acc();    // ACC = lval
          code->sub(data->get_IDR());    // ACC = lval - IDR = lval - rval
        } else {
          lavid->load_value_to_acc();    // ACC = lval
          code->sub(rid->get_addr());    // ACC = lval - rval
        }
      } else {
        if (TArrayVariableIdentifier *ravid = dynamic_cast<TArrayVariableIdentifier*>(rid)) {
          ravid->load_value_to_acc();   // ACC = rval
          code->store(data->get_VLR()); // VLR = rval
          lid->load_value_to_acc();     // ACC = lval
          code->sub(data->get_VLR());   // ACC = lval - VLR = lval - rval
        } else { 
          code->load(lid->get_addr()); // ACC = lval 
          code->sub(rid->get_addr());  // ACC = lval - rval
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

void NumberValue::insert_to_VLR() {
  code->insert_to_acc(this->value);
  code->store(data->get_VLR());
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

void IdentifierValue::insert_to_VLR() {
  // this->identifier->
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

void TVariableIdentifier::load_value_to_acc() {
  code->load(this->variable->get_addr());
}

void TVariableIdentifier::load_value_to_idr() {
  this->load_value_to_acc();
  code->store(data->get_IDR());
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

void TArrayVariableIdentifier::load_value_to_acc() {
  code->insert_to_acc(this->array->get_norm_addr());
  code->add(this->variable->get_addr());
  code->loadi(0);
}

void TArrayVariableIdentifier::load_value_to_idr() {
  this->load_value_to_acc();
  code->store(data->get_IDR());
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

void TArrayNumIdentifier::load_value_to_acc() {
  code->load(this->array->get_addr(this->num_value));
}

void TArrayNumIdentifier::load_value_to_idr() {
  code->load(this->array->get_addr(this->num_value));
  code->store(data->get_IDR());
}
