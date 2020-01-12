#include "types.hpp"
#include "code.hpp"

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
    code->lshift();
    code->store(rid->get_addr());
    lid->load_value_to_acc();
    code->rshift();
    code->store(lid->get_addr());
    code->jump(j);
  code->insert_jump_address(j, code->get_instruction_count());

  code->load(data->get_register(Register::D));
  code->jzero(5); // negate result if sign bit was set 
    code->load(data->get_register(Register::A));
    code->sub(data->get_register(Register::A));
    code->sub(data->get_register(Register::A));
    code->store(data->get_register(Register::A));
  code->load(data->get_register(Register::A));
}

/* TODO: Division */
void TBinaryExpression::div() {
  TIdentifier *lid;
  TIdentifier *rid;
  if (NumberValue *lv = dynamic_cast<NumberValue*>(lvalue)) {
    if (NumberValue *rv = dynamic_cast<NumberValue*>(rvalue)) {
      if (rv->get_value() == 0) code->insert_to_acc(0);
      else code->insert_to_acc(lv->get_value() * rv->get_value());
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

// if (divisor != 0) {
  rid->load_value_to_acc();
  integer j = code->get_instruction_count();
  code->jzero(-j - 1);
    
    code->set_sign_bit(lid, rid); // stores sign bit in Register::D

    code->reset_acc();
    code->store(data->get_register(Register::A));   // result = 0
    code->inc();
    code->store(data->get_register(Register::E));   // multiple = 1
  // while (scaled_divisor < dividend) {
    lid->load_value_to_acc(); 
    code->store(data->get_register(Register::F));   // remain = dividend
    code->sub(rid->get_addr());
    integer k = code->get_instruction_count();
    code->jneg(-k - 1);
    code->jzero(-k - 2);                            // while scaled_divisor < dividend
      rid->load_value_to_acc();
      code->lshift();
      code->store(rid->get_addr());                 // scaled_divisor *= 2
      
      code->load(data->get_register(Register::E));
      code->lshift();
      code->store(data->get_register(Register::E)); // multiple *= 2

      lid->load_value_to_acc();
      code->sub(rid->get_addr());
      code->jump(k);
    code->insert_jump_address(k, code->get_instruction_count());
    code->insert_jump_address(k + 1, code->get_instruction_count());
  // }
    k = code->get_instruction_count();
  // do {
    // if (remain >= scaled_divisor) {
    code->load(data->get_register(Register::F));    // load remain 
    code->sub(rid->get_addr());                     // sub scaled_divisor
    code->jneg(5);                                  
      code->store(data->get_register(Register::F)); // remain -= scaled_divisor
      code->load(data->get_register(Register::A));  
      code->add(data->get_register(Register::E));   
      code->store(data->get_register(Register::A)); // result += multiple
    // }

    code->load(rid->get_addr());
    code->rshift();
    code->store(rid->get_addr());                   // scaled_divisor >>= 1

    code->load(data->get_register(Register::E));
    code->rshift();
    code->store(data->get_register(Register::E));   // multiple >>= 1

    code->jzero(2);
    code->jump(k);
  // } while (multiple != 0)
    
    code->load(data->get_register(Register::D));
    code->jzero(5); // negate result if sign bit was set 
      code->load(data->get_register(Register::A));
      code->sub(data->get_register(Register::A));
      code->sub(data->get_register(Register::A));
      code->store(data->get_register(Register::A));
    code->load(data->get_register(Register::A));

  code->jump(code->get_instruction_count() + 2);
  code->insert_jump_address(j, code->get_instruction_count());
// } else {
  code->reset_acc();
// }
}

/* TODO: Modulo */
void TBinaryExpression::mod() {
  TIdentifier *lid;
  TIdentifier *rid;
  if (NumberValue *lv = dynamic_cast<NumberValue*>(lvalue)) {
    if (NumberValue *rv = dynamic_cast<NumberValue*>(rvalue)) {
      if (rv->get_value() == 0) code->insert_to_acc(0);
      else code->insert_to_acc(lv->get_value() % rv->get_value());
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

// if (divisor != 0) {
  rid->load_value_to_acc();
  integer j = code->get_instruction_count();
  code->jzero(-j - 1);
    
    code->set_sign_bit(lid, rid); // stores sign bit in Register::D

    code->reset_acc();
    code->store(data->get_register(Register::A));   // result = 0
    code->inc();
    code->store(data->get_register(Register::E));   // multiple = 1
  // while (scaled_divisor < dividend) {
    lid->load_value_to_acc(); 
    code->store(data->get_register(Register::F));   // remain = dividend
    code->sub(rid->get_addr());
    integer k = code->get_instruction_count();
    code->jneg(-k - 1);
    code->jzero(-k - 2);                            // while scaled_divisor < dividend
      rid->load_value_to_acc();
      code->lshift();
      code->store(rid->get_addr());                 // scaled_divisor *= 2
      
      code->load(data->get_register(Register::E));
      code->lshift();
      code->store(data->get_register(Register::E)); // multiple *= 2

      lid->load_value_to_acc();
      code->sub(rid->get_addr());
      code->jump(k);
    code->insert_jump_address(k, code->get_instruction_count());
    code->insert_jump_address(k + 1, code->get_instruction_count());
  // }
    k = code->get_instruction_count();
  // do {
    // if (remain >= scaled_divisor) {
    code->load(data->get_register(Register::F));    // load remain 
    code->sub(rid->get_addr());                     // sub scaled_divisor
    code->jneg(5);                                  
      code->store(data->get_register(Register::F)); // remain -= scaled_divisor
      code->load(data->get_register(Register::A));  
      code->add(data->get_register(Register::E));   
      code->store(data->get_register(Register::A)); // result += multiple
    // }

    code->load(rid->get_addr());
    code->rshift();
    code->store(rid->get_addr());                   // scaled_divisor >>= 1

    code->load(data->get_register(Register::E));
    code->rshift();
    code->store(data->get_register(Register::E));   // multiple >>= 1

    code->jzero(2);
    code->jump(k);
  // } while (multiple != 0)
    
    code->load(data->get_register(Register::D));
    code->jzero(5); // negate result if sign bit was set 
      code->load(data->get_register(Register::F));
      code->sub(data->get_register(Register::F));
      code->sub(data->get_register(Register::F));
      code->store(data->get_register(Register::F));
    code->load(data->get_register(Register::F));

  code->jump(code->get_instruction_count() + 2);
  code->insert_jump_address(j, code->get_instruction_count());
// } else {
  code->reset_acc();
// }
}
