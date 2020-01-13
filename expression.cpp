#include "types.hpp"
#include "code.hpp"

#include <math.h>

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
  // TODO: load_expr should have identifier as a parameter to use it as result register
  //  
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

void TBinaryExpression::plus() { // TODO refactor
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

void TBinaryExpression::minus() { // TODO refactor
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

void TBinaryExpression::times() {
  if (NumberValue *lv = dynamic_cast<NumberValue*>(lvalue)) {
    if (NumberValue *rv = dynamic_cast<NumberValue*>(rvalue)) {
      code->insert_to_acc(lv->get_value() * rv->get_value());
      return;
    }
  }
  lvalue->store_in_register(Register::B);
  rvalue->store_in_register(Register::C);
  TIdentifier *lid = new TVariableIdentifier(new Variable(data->get_register(Register::B)));
  TIdentifier *rid = new TVariableIdentifier(new Variable(data->get_register(Register::C)));

  code->set_sign_bit(lid, rid);                     // stores sign bit in Register::D
                                                      
  code->reset_acc();
  code->store(data->get_register(Register::A));
// while (a != 0) {
  lid->load_value_to_acc();
  // if (a & 1) {
    integer j = code->get_instruction_count();
    code->jzero(-j - 1); // IDEA jzero should return its addrs
    code->parity_test(lid);
    code->jzero(4);
      code->load(data->get_register(Register::A));
      code->add(rid->get_addr());
      code->store(data->get_register(Register::A)); // result += b
  // }
    rid->load_value_to_acc();
    code->lshift();
    code->store(rid->get_addr());                   // a >>= 1
    lid->load_value_to_acc();
    code->rshift();
    code->store(lid->get_addr());                   // b <<= 1
    code->jump(j);
  code->insert_jump_address(j, code->get_instruction_count());
// }
// if (sign_bit) {
  code->load(data->get_register(Register::D));
  code->jzero(5); 
    code->load(data->get_register(Register::A));
    code->sub(data->get_register(Register::A));
    code->sub(data->get_register(Register::A));
    code->store(data->get_register(Register::A));   // result *= -1
// }
  code->load(data->get_register(Register::A));
}

void TBinaryExpression::div() {
  if (NumberValue *lv = dynamic_cast<NumberValue*>(lvalue)) {
    if (NumberValue *rv = dynamic_cast<NumberValue*>(rvalue)) {
      if (rv->get_value() == 0) code->reset_acc();
      else code->insert_to_acc(floor((double) lv->get_value() / (double) rv->get_value()));
      return;
    }
  }
  
  lvalue->store_in_register(Register::B);
  rvalue->store_in_register(Register::C);
  TIdentifier *lid = new TVariableIdentifier(new Variable(data->get_register(Register::B)));
  TIdentifier *rid = new TVariableIdentifier(new Variable(data->get_register(Register::C)));

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
    
  // if (sign_bit) {
    code->load(data->get_register(Register::D));
    integer l = code->get_instruction_count();
    code->jzero(-l - 1);
      code->load(data->get_register(Register::A));
      code->sub(data->get_register(Register::A));
      code->sub(data->get_register(Register::A));
      code->store(data->get_register(Register::A));                 // negate result
      code->load(data->get_register(Register::F));
    // if (remain > 0) {
      code->jzero(4);
        code->load(data->get_register(Register::A));
        code->dec();                                                // result--
        code->store(data->get_register(Register::A));
    // }
    code->insert_jump_address(l, code->get_instruction_count());    
    code->load(data->get_register(Register::A));
  // }


  code->jump(code->get_instruction_count() + 2);
  code->insert_jump_address(j, code->get_instruction_count());
// } else {
  code->reset_acc();
// }
}

void TBinaryExpression::mod() {
  if (NumberValue *lv = dynamic_cast<NumberValue*>(lvalue)) {
    if (NumberValue *rv = dynamic_cast<NumberValue*>(rvalue)) {
      if (rv->get_value() == 0) code->insert_to_acc(0);
      else code->insert_to_acc(((lv->get_value() % rv->get_value()) + rv->get_value()) % rv->get_value());
      return;
    }
  }

  lvalue->store_in_register(Register::B);
  rvalue->store_in_register(Register::C);
  TIdentifier *lid = new TVariableIdentifier(new Variable(data->get_register(Register::B)));
  TIdentifier *rid = new TVariableIdentifier(new Variable(data->get_register(Register::C)));

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
    k = code->get_instruction_count();
  // if (sign_bit) {
    code->jzero(-k - 1); // negate result if sign bit was set 
      rvalue->load_value();
      code->jneg(2); code->jump(code->get_instruction_count() + 3); // b < 0
        code->add(data->get_register(Register::F));
        code->store(data->get_register(Register::F));
      
      lvalue->load_value();
      integer l = code->get_instruction_count();
      code->jneg(2); code->jump(); // a < 0
        rvalue->load_value();
        code->sub(data->get_register(Register::F));
        code->store(data->get_register(Register::F));
      code->insert_jump_address(l + 1 , 1 + code->get_instruction_count());
      integer m = code->get_instruction_count();
      code->jump();
    code->insert_jump_address(k, code->get_instruction_count());
  // } else { 
      lvalue->load_value();
      code->jpos(5);
        code->load(data->get_register(Register::F));
        code->sub(data->get_register(Register::F));
        code->sub(data->get_register(Register::F));
        code->store(data->get_register(Register::F));
    code->insert_jump_address(m, code->get_instruction_count());
  // }



    code->load(data->get_register(Register::F));

  code->jump(code->get_instruction_count() + 2);
  code->insert_jump_address(j, code->get_instruction_count());
// } else {
  code->reset_acc();
// }
}
