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
  this->value->load_value();
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
        avid->load_value_to_register(Register::IDR);            // IDR = rval
        code->insert_to_acc(lv->get_value()); // ACC = lval
        code->add(data->get_register(Register::IDR));           // ACC = lval + rval
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
        code->add(data->get_register(Register::VLR));           // ACC = lval + rval           
      } else {
        code->insert_to_acc(rv->get_value()); // ACC = rval
        code->add(lid->get_addr());           // ACC = rval + lval
      }
    } else {
      TIdentifier *lid = static_cast<IdentifierValue*>(lvalue)->get_identifier();
      TIdentifier *rid = static_cast<IdentifierValue*>(rvalue)->get_identifier();

      if (TArrayVariableIdentifier *lavid = dynamic_cast<TArrayVariableIdentifier*>(lid)) {
        if (TArrayVariableIdentifier *ravid = dynamic_cast<TArrayVariableIdentifier*>(rid)) {      
          ravid->load_value_to_register(Register::IDR); // IDR = rval
          lavid->load_value_to_acc(); // ACC = lval
          code->add(data->get_register(Register::IDR)); // ACC = lval + IDR = lval + rval
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


void TBinaryExpression::minus() {
  if (NumberValue *lv = dynamic_cast<NumberValue*>(lvalue)) {
    if (NumberValue *rv = dynamic_cast<NumberValue*>(rvalue)) {
      code->insert_to_acc(lv->get_value() - rv->get_value());
    }  else {
      TIdentifier *rid = static_cast<IdentifierValue*>(rvalue)->get_identifier();
      if (TArrayVariableIdentifier *avid = dynamic_cast<TArrayVariableIdentifier*>(rid)) {
        avid->load_value_to_register(Register::IDR);            // IDR = rval
        code->insert_to_acc(lv->get_value()); // ACC = lval
        code->sub(data->get_register(Register::IDR));           // ACC = lval - rval
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
        code->sub(data->get_register(Register::VLR));           // ACC = lval - rval           
      } else {
        code->insert_to_acc(rv->get_value()); // ACC = rval
        code->store(data->get_register(Register::VLR));         // VLR = rval
        lid->load_value_to_acc();             // ACC = lval
        code->sub(data->get_register(Register::VLR));           // ACC = lval - rval
      }
    } else {
      TIdentifier *lid = static_cast<IdentifierValue*>(lvalue)->get_identifier();
      TIdentifier *rid = static_cast<IdentifierValue*>(rvalue)->get_identifier();

      if (TArrayVariableIdentifier *lavid = dynamic_cast<TArrayVariableIdentifier*>(lid)) {
        if (TArrayVariableIdentifier *ravid = dynamic_cast<TArrayVariableIdentifier*>(rid)) {      
          ravid->load_value_to_register(Register::IDR);    // IDR = rval
          lavid->load_value_to_acc();    // ACC = lval
          code->sub(data->get_register(Register::IDR));    // ACC = lval - IDR = lval - rval
        } else {
          lavid->load_value_to_acc();    // ACC = lval
          code->sub(rid->get_addr());    // ACC = lval - rval
        }
      } else {
        if (TArrayVariableIdentifier *ravid = dynamic_cast<TArrayVariableIdentifier*>(rid)) {
          ravid->load_value_to_acc();   // ACC = rval
          code->store(data->get_register(Register::VLR)); // VLR = rval
          lid->load_value_to_acc();     // ACC = lval
          code->sub(data->get_register(Register::VLR));   // ACC = lval - VLR = lval - rval
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
  TIdentifier *lid;
  TIdentifier *rid;
  if (NumberValue *lv = dynamic_cast<NumberValue*>(lvalue)) {
    if (NumberValue *rv = dynamic_cast<NumberValue*>(rvalue)) {
      code->insert_to_acc(lv->get_value() * rv->get_value());
      return;
    }
    lv->insert_to_VLR();
    lid = new TVariableIdentifier(new Variable(data->get_register(Register::VLR)));
    
    rid = static_cast<IdentifierValue*>(rvalue)->get_identifier();
    if (TArrayVariableIdentifier *ravid = dynamic_cast<TArrayVariableIdentifier*>(rid)) {
      ravid->load_value_to_register(Register::C);
      rid = new TVariableIdentifier(new Variable(data->get_register(Register::C)));
    }
  } else {
    lid = static_cast<IdentifierValue*>(lvalue)->get_identifier();
    if (TArrayVariableIdentifier *lavid = dynamic_cast<TArrayVariableIdentifier*>(lid)) {
      lavid->load_value_to_register(Register::B);
      lid = new TVariableIdentifier(new Variable(data->get_register(Register::B)));
    }
    
    if (NumberValue *rv = dynamic_cast<NumberValue*>(rvalue)) {
      rv->insert_to_VLR();
      rid = new TVariableIdentifier(new Variable(data->get_register(Register::VLR)));
    } else {
      rid = static_cast<IdentifierValue*>(rvalue)->get_identifier();
      if (TArrayVariableIdentifier *ravid = dynamic_cast<TArrayVariableIdentifier*>(rid)) {
        ravid->load_value_to_register(Register::C);
        rid = new TVariableIdentifier(new Variable(data->get_register(Register::C)));
      }
    }
  }

  code->set_sign_bit(lid, rid); // stores sign bit in Register::D

  code->reset_acc();
  code->store(data->get_register(Register::A));
  lid->load_value_to_acc();
    integer j = code->get_instruction_count();
    code->jzero(-j - 1); // IDEA jzero should return its addrs
    code->parity_test(lid);
    code->jzero(4);
      code->load(data->get_register(Register::A));
      code->add(rid->get_addr());
      code->store(data->get_register(Register::A));
    rid->load_value_to_acc();
    code->add(rid->get_addr());
    code->store(rid->get_addr());
    lid->load_value_to_acc();
    code->rshift();
    code->store(lid->get_addr());
    code->jump(j);
  code->insert_jump_address(j, code->get_instruction_count());

  code->load(data->get_register(Register::D));
  code->jzero(5);
    code->load(data->get_register(Register::A));
    code->sub(data->get_register(Register::A));
    code->sub(data->get_register(Register::A));
    code->store(data->get_register(Register::A));
  code->load(data->get_register(Register::A));
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
  code->store(data->get_register(Register::VLR));
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

TIdentifier* IdentifierValue::get_identifier() {
  return this->identifier;
}

/*
 ********************
 *    IDENTIFIER    *
 ********************
 */

void TIdentifier::load_value_to_register(Register reg) {
  this->load_value_to_acc();
  code->store(data->get_register(reg));
}


TVariableIdentifier::TVariableIdentifier(ident var_name) {
  this->variable = static_cast<Variable *>(data->get_symbol(var_name));
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
  if (!is_loaded) {
    this->load_value_to_acc();
  }
  code->sub(this->get_addr());
  code->sub(this->get_addr());
  code->store(this->get_addr());
}


TArrayVariableIdentifier::TArrayVariableIdentifier(ident arr_name, ident var_name) {
  this->array = static_cast<Array *>(data->get_symbol(arr_name));
  this->variable = static_cast<Variable *>(data->get_symbol(var_name));
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
    this->load_value_to_acc();
  }
  this->load_addr_to_register(Register::IDR);
  code->sub(data->get_register(Register::IDR));
  code->sub(data->get_register(Register::IDR));
  code->store(data->get_register(Register::IDR));
}


TArrayNumIdentifier::TArrayNumIdentifier(ident arr_name, integer num_value) {
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
  if (!is_loaded) {
    this->load_value_to_acc();
  }
  code->sub(this->get_addr());
  code->sub(this->get_addr());
  code->store(this->get_addr());
}
