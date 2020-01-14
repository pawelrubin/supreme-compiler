#include "types.hpp"
#include "code.hpp"

#include <math.h>
#include <iostream>

TValueExpression::TValueExpression(TValue *value) {
  this->value = value;
}

void TValueExpression::load_expr(TIdentifier* result) {
  if (TArrayVariableIdentifier *id = dynamic_cast<TArrayVariableIdentifier*>(result)) {
    id->load_addr_to_register(Register::IDR1);
    this->value->load_value();
    code->storei(data->get_register(Register::IDR1));
  } else {
    this->value->load_value();
    code->store(result->get_addr());
  }
}

TBinaryExpression::TBinaryExpression(TValue *lvalue, TValue *rvalue, BinaryOperator op) {
  this->lvalue = lvalue;
  this->rvalue = rvalue;
  this->op = op;
}

void TBinaryExpression::load_expr(TIdentifier* result) {

  if (NumberValue *rv = dynamic_cast<NumberValue*>(rvalue)) {
    integer num = rv->get_value();
    if (num < llabs(10)) {
      if (IdentifierValue* ival = dynamic_cast<IdentifierValue*>(lvalue) ) {
        if (ival->get_identifier()->get_name() == result->get_name()) {
          ident id = result->get_name();
          std::cerr << " X ASSIGN X EXPRESSION n: " << id << " ASSIGN " << id << " op " << std::to_string(num) << std::endl;
          if (TArrayVariableIdentifier* avid = dynamic_cast<TArrayVariableIdentifier*>(result)) {
            avid->load_addr_to_register(Register::IDR);
          }
          switch (this->op) {
          case BinaryOperator::PLUS:
            result->load_value_to_acc();
            for (int i = 0; i < num; i++) {
              code->inc();
            }
            if (TArrayVariableIdentifier* avid = dynamic_cast<TArrayVariableIdentifier*>(result)) {
              code->storei(data->get_register(Register::IDR));
            } else {
              code->store(result->get_addr());
            }
            return;
          case BinaryOperator::MINUS:
            result->load_value_to_acc();
            for (int i = 0; i < num; i++) {
              code->dec();
            }
            if (TArrayVariableIdentifier* avid = dynamic_cast<TArrayVariableIdentifier*>(result)) {
              code->storei(data->get_register(Register::IDR));
            } else {
              code->store(result->get_addr());
            }
            return;
          }
        }
      } 
    }    
  }

  if (TArrayVariableIdentifier *id = dynamic_cast<TArrayVariableIdentifier*>(result)) {
    id->load_addr_to_register(Register::IDR1);
  }

  switch (this->op) {
  case BinaryOperator::PLUS:
    if (TArrayVariableIdentifier *id = dynamic_cast<TArrayVariableIdentifier*>(result)) {
      id->load_addr_to_register(Register::IDR1);
      this->plus();
      code->storei(data->get_register(Register::IDR1));
    } else {
      this->plus();
      code->store(result->get_addr());
    }
    break;
  
  case BinaryOperator::MINUS:
    if (TArrayVariableIdentifier *id = dynamic_cast<TArrayVariableIdentifier*>(result)) {
      id->load_addr_to_register(Register::IDR1);
      this->minus();
      code->storei(data->get_register(Register::IDR1));
    } else {
      this->minus();
      code->store(result->get_addr());
    }
    break;

  case BinaryOperator::TIMES:
    if (TArrayVariableIdentifier *id = dynamic_cast<TArrayVariableIdentifier*>(result)) {
      id->load_addr_to_register(Register::IDR1);
      this->times(data->get_register(Register::IDR1));
    } else {
      this->times(result->get_addr());
    }
    break;
  
  case BinaryOperator::DIV:
    if (TArrayVariableIdentifier *id = dynamic_cast<TArrayVariableIdentifier*>(result)) {
      id->load_addr_to_register(Register::IDR1);
      this->div(data->get_register(Register::IDR1));
    } else {
      this->div(result->get_addr());
    }
    break;
  
  case BinaryOperator::MOD:
    if (TArrayVariableIdentifier *id = dynamic_cast<TArrayVariableIdentifier*>(result)) {
      id->load_addr_to_register(Register::IDR1);
      this->mod(data->get_register(Register::IDR1));
    } else {
      this->mod(result->get_addr());
    }
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

void TBinaryExpression::times(integer identifier_address) {
  if (NumberValue *lv = dynamic_cast<NumberValue*>(lvalue)) {
    if (NumberValue *rv = dynamic_cast<NumberValue*>(rvalue)) {
      code->insert_to_acc(lv->get_value() * rv->get_value());
      if (identifier_address == data->get_register(Register::IDR1)) {
        code->storei(identifier_address);
      } else {
        code->store(identifier_address);
      }
      return;
    }
  }
  lvalue->store_in_register(Register::B);
  rvalue->store_in_register(Register::C);
  TIdentifier *lid = new TVariableIdentifier(new Variable(data->get_register(Register::B)));
  TIdentifier *rid = new TVariableIdentifier(new Variable(data->get_register(Register::C)));

  code->set_sign_bit(lid, rid);                     // stores sign bit in Register::D
                                                      
  code->reset_acc();
  
  integer addr_for_result = identifier_address;
  if (identifier_address == data->get_register(Register::IDR1)) {
    addr_for_result = data->get_register(Register::A);
  }
  code->store(addr_for_result);
// while (a != 0) {
  lid->load_value_to_acc();
  // if (a & 1) {
  integer j = code->jzero();
    code->parity_test(lid);
    integer k = code->jzero();
      code->load(addr_for_result);
      code->add(rid->get_addr());
      code->store(addr_for_result); // result += b
    code->insert_jump_address(k);
  // }
    rid->load_value_to_acc();
    code->lshift();
    code->store(rid->get_addr());                   // a >>= 1
    lid->load_value_to_acc();
    code->rshift();
    code->store(lid->get_addr());                   // b <<= 1
    code->jump(j);
  code->insert_jump_address(j);
// }
// if (sign_bit) {
  code->load(data->get_register(Register::D));
  j = code->jzero(); 
    code->load(addr_for_result);
    code->sub(addr_for_result);
    code->sub(addr_for_result);
    code->store(addr_for_result);   // result *= -1
  code->insert_jump_address(j);
// }
  if (addr_for_result == data->get_register(Register::A)) {
    code->load(data->get_register(Register::A));
    code->storei(identifier_address);
  }
}

void TBinaryExpression::div(integer identifier_address) {
  if (NumberValue *lv = dynamic_cast<NumberValue*>(lvalue)) {
    if (NumberValue *rv = dynamic_cast<NumberValue*>(rvalue)) {
      if (rv->get_value() == 0) code->reset_acc();
      else code->insert_to_acc(floor((double) lv->get_value() / (double) rv->get_value()));
      if (identifier_address == data->get_register(Register::IDR1)) {
        code->storei(identifier_address);
      } else {
        code->store(identifier_address);
      }
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
  code->jzero();
    
    code->set_sign_bit(lid, rid); // stores sign bit in Register::D

    code->reset_acc();
    integer addr_for_result = identifier_address;
    if (identifier_address == data->get_register(Register::IDR1)) {
      addr_for_result = data->get_register(Register::A);
    }
    code->store(addr_for_result);   // result = 0
    code->inc();
    code->store(data->get_register(Register::E));   // multiple = 1
  // while (scaled_divisor < dividend) {
    lid->load_value_to_acc(); 
    code->store(data->get_register(Register::F));   // remain = dividend
    code->sub(rid->get_addr());
    integer k = code->jneg();
    code->jzero();                            // while scaled_divisor < dividend
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
      code->load(addr_for_result);  
      code->add(data->get_register(Register::E));   
      code->store(addr_for_result); // result += multiple
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
    integer l = code->jzero();
      code->load(addr_for_result);
      code->sub(addr_for_result);
      code->sub(addr_for_result);
      code->store(addr_for_result);                 // negate result
      code->load(data->get_register(Register::F));
    // if (remain > 0) {
      code->jzero(4);
        code->load(addr_for_result);
        code->dec();                                                // result--
        code->store(addr_for_result);
    code->insert_jump_address(l, code->get_instruction_count());    
    // }
    
    if (addr_for_result == data->get_register(Register::A)) {
      code->load(data->get_register(Register::A));
      code->storei(identifier_address);
    }
  
  // }
  integer z = code->jump();
  code->insert_jump_address(j, code->get_instruction_count());
// } else {
    if (addr_for_result == data->get_register(Register::A)) {
      code->load(data->get_register(Register::A));
      code->reset_acc();
      code->storei(identifier_address);
    } else {
      code->reset_acc();
      code->store(addr_for_result);
    }
  code->insert_jump_address(z, code->get_instruction_count());
// }
}

void TBinaryExpression::mod(integer identifier_address) {
  if (NumberValue *lv = dynamic_cast<NumberValue*>(lvalue)) {
    if (NumberValue *rv = dynamic_cast<NumberValue*>(rvalue)) {
      if (rv->get_value() == 0) code->insert_to_acc(0);
      else code->insert_to_acc(((lv->get_value() % rv->get_value()) + rv->get_value()) % rv->get_value());
      if (identifier_address == data->get_register(Register::IDR1)) {
        code->storei(identifier_address);
      } else {
        code->store(identifier_address);
      }
      return;
    }
  }

  lvalue->store_in_register(Register::B);
  rvalue->store_in_register(Register::C);
  TIdentifier *lid = new TVariableIdentifier(new Variable(data->get_register(Register::B)));
  TIdentifier *rid = new TVariableIdentifier(new Variable(data->get_register(Register::C)));

  integer addr_for_result = identifier_address;
  if (identifier_address == data->get_register(Register::IDR1)) {
    addr_for_result = data->get_register(Register::F);
  }

// if (divisor != 0) {
  rid->load_value_to_acc();
  integer j = code->jzero();
    
    code->set_sign_bit(lid, rid); // stores sign bit in Register::D

    code->reset_acc();
    code->store(data->get_register(Register::A));   // result = 0
    code->inc();
    code->store(data->get_register(Register::E));   // multiple = 1
  // while (scaled_divisor < dividend) {
    lid->load_value_to_acc(); 
    code->store(addr_for_result);   // remain = dividend
    code->sub(rid->get_addr());
    integer k = code->jneg();
    code->jzero();                            // while scaled_divisor < dividend
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
    code->load(addr_for_result);    // load remain 
    code->sub(rid->get_addr());                     // sub scaled_divisor
    code->jneg(5);                                  
      code->store(addr_for_result); // remain -= scaled_divisor
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
  // if (sign_bit) {
    k = code->jzero(); 
      rvalue->load_value();
      code->jneg(2); code->jump(code->get_instruction_count() + 3); // b < 0
        code->add(addr_for_result);
        code->store(addr_for_result);
      
      lvalue->load_value();
      integer l = code->get_instruction_count();
      code->jneg(2); code->jump(); // a < 0
        rvalue->load_value();
        code->sub(addr_for_result);
        code->store(addr_for_result);
      code->insert_jump_address(l + 1 , 1 + code->get_instruction_count());
      integer m = code->get_instruction_count();
      code->jump();
    code->insert_jump_address(k, code->get_instruction_count());
  // } else { 
      lvalue->load_value();
      code->jpos(5);
        code->load(addr_for_result);
        code->sub(addr_for_result);
        code->sub(addr_for_result);
        code->store(addr_for_result);
    code->insert_jump_address(m, code->get_instruction_count());
  // }

    if (addr_for_result == data->get_register(Register::F)) {
      code->load(data->get_register(Register::F));
      code->storei(identifier_address);
    }

  integer z = code->jump();
  code->insert_jump_address(j, code->get_instruction_count());
// } else {
    if (addr_for_result == data->get_register(Register::F)) {
      code->load(data->get_register(Register::F));
      code->reset_acc();
      code->storei(identifier_address);
    } else {
      code->reset_acc();
      code->store(addr_for_result);
    }
  code->insert_jump_address(z, code->get_instruction_count());
// }
}
