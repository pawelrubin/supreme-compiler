#include "types.hpp"
#include "code.hpp"

#include <math.h>
#include <iostream>

InstructionVector TValueExpression::load_expr(TIdentifier* result) {
  InstructionVector instructions;
  if (auto id = dynamic_cast<TArrayVariableIdentifier*>(result)) {
    instructions
    .append(id->load_addr_to_register(Register::IDR1))
    .append(this->value->load_value())
    .push(new Storei(data->get_register(Register::IDR1)));
  } else {
    instructions
    .append(this->value->load_value())
    .push(new Store(result->get_addr()));
  }
  return instructions;
}

InstructionVector TBinaryExpression::load_expr(TIdentifier* result) {
  // y = x OP x TODO refactor
  InstructionVector instructions;
  // if (auto lid = dynamic_cast<IdentifierValue*>(lvalue)) {
  //   if (auto rid = dynamic_cast<IdentifierValue*>(rvalue)) {
  //     if (lid->get_identifier()->get_name() == rid->get_identifier()->get_name()) {
  //       std::cerr<<"x OP x"<<std::endl;
  //       switch (this->op){
  //       case BinaryOperator::PLUS:
  //         if (TArrayVariableIdentifier *id = dynamic_cast<TArrayVariableIdentifier*>(result)) {
  //           instructions
  //           .append(id->load_addr_to_register(Register::IDR1))
  //           .append(lid->load_value())
  //           .append(code->lshift())
  //           .push(new Storei(data->get_register(Register::IDR1)));
  //         } else {
  //           instructions
  //           .append(lid->load_value())
  //           .append(code->lshift())
  //           .push(new Store(result->get_addr()));
  //         }
  //         return instructions;
  //       case BinaryOperator::MINUS:
  //         if (TArrayVariableIdentifier *id = dynamic_cast<TArrayVariableIdentifier*>(result)) {
  //           instructions
  //           .append(id->load_addr_to_register(Register::IDR1))
  //           .push(new Sub(0))
  //           .push(new Storei(data->get_register(Register::IDR1)));
  //         } else {
  //           instructions
  //           .push(new Sub(0))
  //           .push(new Store(result->get_addr()));
  //         }
  //         return instructions;
  //       case BinaryOperator::DIV:
  //         if (TArrayVariableIdentifier *id = dynamic_cast<TArrayVariableIdentifier*>(result)) {
  //           instructions
  //           .append(id->load_addr_to_register(Register::IDR1))
  //           .push(new Sub(0))
  //           .push(new Inc())
  //           .push(new Storei(data->get_register(Register::IDR1)));
  //         } else {
  //           instructions
  //           .push(new Sub(0))
  //           .push(new Inc())
  //           .push(new Store(result->get_addr()));
  //         }
  //         return instructions;
  //       case BinaryOperator::MOD:
  //         if (TArrayVariableIdentifier *id = dynamic_cast<TArrayVariableIdentifier*>(result)) {
  //           instructions
  //           .append(id->load_addr_to_register(Register::IDR1))
  //           .push(new Sub(0))
  //           .push(new Storei(data->get_register(Register::IDR1)));
  //         } else {
  //           instructions
  //           .push(new Sub(0))
  //           .push(new Store(result->get_addr()));
  //         }
  //         return instructions;
  //       default:
  //         break;
  //       }
  //     }
  //   }
  // }

  // x OP num
  if (auto rv = dynamic_cast<NumberValue*>(rvalue)) {
    if (auto lv = dynamic_cast<IdentifierValue*>(lvalue)) {
      integer num = rv->get_value();
      if (!(
        ((op == BinaryOperator::MINUS or op == BinaryOperator::PLUS) and (llabs(num) > 10)) or 
        ((op == BinaryOperator::MOD or op == BinaryOperator::DIV) and (llabs(num) != 2))
      )) {         
        auto lid = lv->get_identifier();
        if (dynamic_cast<TArrayVariableIdentifier*>(result)) {
          instructions.append(result->load_addr_to_register(Register::IDR1));
        }
        if (dynamic_cast<TArrayVariableIdentifier*>(lid)) {
          instructions.append(lid->load_addr_to_register(Register::IDR2));
        }

        switch (this->op) {
          case BinaryOperator::PLUS:
            instructions.append(lid->load());
            for (int i = 0; i < num; i++) {
              instructions.push(new Inc());
            }
            if (dynamic_cast<TArrayVariableIdentifier*>(result)) {
              instructions.push(new Storei(data->get_register(Register::IDR1)));
            } else {
              instructions.push(new Store(result->get_addr()));
            }
            return instructions;
          case BinaryOperator::MINUS:
            instructions.append(lid->load());
            for (int i = 0; i < num; i++) {
              instructions.push(new Dec());
            }
            if (dynamic_cast<TArrayVariableIdentifier*>(result)) {
              instructions.push(new Storei(data->get_register(Register::IDR1)));
            } else {
              instructions.push(new Store(result->get_addr()));
            }
            return instructions; 
          case BinaryOperator::TIMES:
            if (code->is_power_of_two(num)) {
              instructions.append(lid->load());
              while (num > 1) {
                instructions.append(code->lshift());
                num /= 2;
              }
            }
            if (dynamic_cast<TArrayVariableIdentifier*>(result)) {
              instructions.push(new Storei(data->get_register(Register::IDR1)));
            } else {
              instructions.push(new Store(result->get_addr()));
            }
            return instructions;
          case BinaryOperator::DIV:
            if (num == 2) {
              instructions.append(lid->load());
              auto j = new Jpos();
                instructions.push(j)
                .push(new Sub(0));
                if (dynamic_cast<TArrayVariableIdentifier*>(lid)) {
                  instructions.push(new Sub(data->get_register(Register::IDR2)));
                } else {
                  instructions.push(new Sub(lid->get_addr()));
                }
                auto k = new Jump();
                instructions.push(k)
              .push(new NOP(j))
              .append(code->rshift())
              .push(new NOP(k));
            } else if (num == -2) {
              instructions.push(new Sub(0));
              if (dynamic_cast<TArrayVariableIdentifier*>(lid)) {
                instructions.push(new Sub(data->get_register(Register::IDR2)));
              } else {
                instructions.push(new Sub(lid->get_addr()));
              }
              instructions.append(code->rshift());
            }
            if (dynamic_cast<TArrayVariableIdentifier*>(result)) {
              instructions.push(new Storei(data->get_register(Register::IDR1)));
            } else {
              instructions.push(new Store(result->get_addr()));
            }
            return instructions;
          case BinaryOperator::MOD:
            if (num == 2) {
              instructions.append(code->parity_test(lid));
              auto j = new Jpos();
                instructions.push(j)
                .push(new Inc())
                .push(new Inc())
              .push(new NOP(j));
            } else if (num == -2) {
              instructions.append(code->parity_test(lid));
            }
            if (dynamic_cast<TArrayVariableIdentifier*>(result)) {
              instructions.push(new Storei(data->get_register(Register::IDR1)));
            } else {
              instructions.push(new Store(result->get_addr()));
            }
            return instructions;
        }
      }
    }
  }
  
  switch (this->op) {
  case BinaryOperator::PLUS:
    if (TArrayVariableIdentifier *id = dynamic_cast<TArrayVariableIdentifier*>(result)) {
      instructions.append(id->load_addr_to_register(Register::IDR1))
      .append(this->plus())
      .push(new Storei(data->get_register(Register::IDR1)));
    } else {
      instructions.append(this->plus())
      .push(new Store(result->get_addr()));
    }
    break;
  
  case BinaryOperator::MINUS:
    if (TArrayVariableIdentifier *id = dynamic_cast<TArrayVariableIdentifier*>(result)) {
      instructions.append(id->load_addr_to_register(Register::IDR1))
      .append(this->minus())
      .push(new Storei(data->get_register(Register::IDR1)));
    } else {
      instructions.append(this->minus())
      .push(new Store(result->get_addr()));
    }
    break;

  case BinaryOperator::TIMES:
    if (TArrayVariableIdentifier *id = dynamic_cast<TArrayVariableIdentifier*>(result)) {
      instructions.append(id->load_addr_to_register(Register::IDR1))
      .append(this->times(data->get_register(Register::IDR1)));
    } else {
      instructions.append(this->times(result->get_addr()));
    }
    break;
  
  case BinaryOperator::DIV:
    if (TArrayVariableIdentifier *id = dynamic_cast<TArrayVariableIdentifier*>(result)) {
      instructions.append(id->load_addr_to_register(Register::IDR1))
      .append(this->div(data->get_register(Register::IDR1)));
    } else {
      instructions.append(this->div(result->get_addr()));
    }
    break;
  
  case BinaryOperator::MOD:
    if (TArrayVariableIdentifier *id = dynamic_cast<TArrayVariableIdentifier*>(result)) {
      instructions.append(id->load_addr_to_register(Register::IDR1))
      .append(this->mod(data->get_register(Register::IDR1)));
    } else {
      instructions.append(this->mod(result->get_addr()));
    }
    break;
  }

  return instructions;
}

InstructionVector TBinaryExpression::plus() { // TODO refactor
  InstructionVector instructions;

  if (NumberValue *lv = dynamic_cast<NumberValue*>(lvalue)) {
    if (NumberValue *rv = dynamic_cast<NumberValue*>(rvalue)) {
      instructions.append(code->insert_to_acc(lv->get_value() + rv->get_value()));
    } else {
      TIdentifier *rid = static_cast<IdentifierValue*>(rvalue)->get_identifier();
      if (TArrayVariableIdentifier *avid = dynamic_cast<TArrayVariableIdentifier*>(rid)) {
        instructions.append(avid->load_value_to_register(Register::IDR))            // IDR = rval
        .append(code->insert_to_acc(lv->get_value())) // ACC = lval
        .push(new Add(data->get_register(Register::IDR)));           // ACC = lval + rval
      } else {
        instructions.append(code->insert_to_acc(lv->get_value())) // ACC = lval
        .push(new Add(rid->get_addr()));           // ACC = lval + rval
      }
    }
  } else {
    if (NumberValue *rv = dynamic_cast<NumberValue*>(rvalue)) {
      TIdentifier *lid = static_cast<IdentifierValue*>(lvalue)->get_identifier();
      if (TArrayVariableIdentifier *avid = dynamic_cast<TArrayVariableIdentifier*>(lid)) {
        instructions.append(rv->store_in_register(Register::VLR))                  // VLR = rval
        .append(avid->load())                               // ACC = lval
        .push(new Add(data->get_register(Register::VLR)));           // ACC = lval + rval           
      } else {
        instructions.append(code->insert_to_acc(rv->get_value())) // ACC = rval
        .push(new Add(lid->get_addr()));           // ACC = rval + lval
      }
    } else {
      TIdentifier *lid = static_cast<IdentifierValue*>(lvalue)->get_identifier();
      TIdentifier *rid = static_cast<IdentifierValue*>(rvalue)->get_identifier();

      if (TArrayVariableIdentifier *lavid = dynamic_cast<TArrayVariableIdentifier*>(lid)) {
        if (TArrayVariableIdentifier *ravid = dynamic_cast<TArrayVariableIdentifier*>(rid)) {      
          instructions.append(ravid->load_value_to_register(Register::IDR)) // IDR = rval
          .append(lavid->load()) // ACC = lval
          .push(new Add(data->get_register(Register::IDR))); // ACC = lval + IDR = lval + rval
        } else {
          instructions.append(lavid->load()) // ACC = lval
          .push(new Add(rid->get_addr())); // ACC = lval + rval
        }
      } else {
        if (TArrayVariableIdentifier *ravid = dynamic_cast<TArrayVariableIdentifier*>(rid)) {
          instructions.append(ravid->load()) // ACC = rval
          .push(new Add(lid->get_addr())); // ACC = rval + lval
        } else {
          instructions.push(new Load(lid->get_addr())) // ACC = lval 
          .push(new Add(rid->get_addr()));  // ACC = lval + rval
        }
      }
    }
  }

  return instructions;
}

InstructionVector TBinaryExpression::minus() { // TODO refactor
  InstructionVector instructions;

  if (NumberValue *lv = dynamic_cast<NumberValue*>(lvalue)) {
    if (NumberValue *rv = dynamic_cast<NumberValue*>(rvalue)) {
      instructions.append(code->insert_to_acc(lv->get_value() - rv->get_value()));
    }  else {
      TIdentifier *rid = static_cast<IdentifierValue*>(rvalue)->get_identifier();
      if (TArrayVariableIdentifier *avid = dynamic_cast<TArrayVariableIdentifier*>(rid)) {
        instructions.append(avid->load_value_to_register(Register::IDR))            // IDR = rval
        .append(code->insert_to_acc(lv->get_value())) // ACC = lval
        .push(new Sub(data->get_register(Register::IDR)));           // ACC = lval - rval
      } else {
        instructions.append(code->insert_to_acc(lv->get_value())) // ACC = lval
        .push(new Sub(rid->get_addr()));           // ACC = lval - rval
      }
    }
  } else {
    if (NumberValue *rv = dynamic_cast<NumberValue*>(rvalue)) {
      TIdentifier *lid = static_cast<IdentifierValue*>(lvalue)->get_identifier();
      if (TArrayVariableIdentifier *avid = dynamic_cast<TArrayVariableIdentifier*>(lid)) {
        instructions.append(rv->store_in_register(Register::VLR))                  // VLR = rval
        .append(avid->load())            // ACC = lval
        .push(new Sub(data->get_register(Register::VLR)));           // ACC = lval - rval
      } else {
        instructions.append(code->insert_to_acc(rv->get_value())) // ACC = rval
        .push(new Store(data->get_register(Register::VLR)))         // VLR = rval
        .append(lid->load())             // ACC = lval
        .push(new Sub(data->get_register(Register::VLR)));           // ACC = lval - rval
      }
    } else {
      TIdentifier *lid = static_cast<IdentifierValue*>(lvalue)->get_identifier();
      TIdentifier *rid = static_cast<IdentifierValue*>(rvalue)->get_identifier();

      if (TArrayVariableIdentifier *lavid = dynamic_cast<TArrayVariableIdentifier*>(lid)) {
        if (TArrayVariableIdentifier *ravid = dynamic_cast<TArrayVariableIdentifier*>(rid)) {      
          instructions.append(ravid->load_value_to_register(Register::IDR))    // IDR = rval
          .append(lavid->load())    // ACC = lval
          .push(new Sub(data->get_register(Register::IDR)));    // ACC = lval - IDR = lval - rval
        } else {
          instructions.append(lavid->load())    // ACC = lval
          .push(new Sub(rid->get_addr()));    // ACC = lval - rval
        }
      } else {
        if (TArrayVariableIdentifier *ravid = dynamic_cast<TArrayVariableIdentifier*>(rid)) {
          instructions.append(ravid->load())   // ACC = rval
          .push(new Store(data->get_register(Register::VLR))) // VLR = rval
          .append(lid->load())     // ACC = lval
          .push(new Sub(data->get_register(Register::VLR)));   // ACC = lval - VLR = lval - rval
        } else {
          instructions
          .push(new Load(lid->get_addr())) // ACC = lval 
          .push(new Sub(rid->get_addr()));  // ACC = lval - rval
        }
      }
    }
  }

  return instructions;
}

InstructionVector TBinaryExpression::times(integer identifier_address) {
  InstructionVector instructions;
  
  if (NumberValue *lv = dynamic_cast<NumberValue*>(lvalue)) {
    if (NumberValue *rv = dynamic_cast<NumberValue*>(rvalue)) {
      instructions.append(code->insert_to_acc(lv->get_value() * rv->get_value()));
      if (identifier_address == data->get_register(Register::IDR1)) {
        instructions.push(new Storei(identifier_address));
      } else {
        instructions.push(new Store(identifier_address));
      }
      return instructions;
    }
  }
  instructions.append(lvalue->store_in_register(Register::B))
  .append(rvalue->store_in_register(Register::C));

  TIdentifier *lid = new TVariableIdentifier(new Variable(data->get_register(Register::B)));
  TIdentifier *rid = new TVariableIdentifier(new Variable(data->get_register(Register::C)));

  instructions.append(code->set_sign_bit(lid, rid))                     // stores sign bit in Register::D
                                                      
  .push(new Sub(0));
  
  integer addr_for_result = identifier_address;
  if (identifier_address == data->get_register(Register::IDR1)) { 
    addr_for_result = data->get_register(Register::A);
  }
  instructions.push(new Store(addr_for_result))
// while (a != 0) {
  .append(lid->load()); 
  // if (a & 1) {
  auto j = new Jzero();
    instructions.push(j)
    .append(code->parity_test(lid));
    auto k = new Jzero();
      instructions.push(k)
      .push(new Load(addr_for_result))
      .push(new Add(rid->get_addr()))
      .push(new Store(addr_for_result)) // result += b
    .push(new NOP(k))
  // }
    .append(rid->load())
    .append(code->lshift())
    .push(new Store(rid->get_addr()))                   // a >>= 1
    .append(lid->load())
    .append(code->rshift())
    .push(new Store(lid->get_addr()))                   // b <<= 1
    .push(new Jump(j))
  .push(new NOP(j))
// }
// if (sign_bit) {
  .push(new Load(data->get_register(Register::D)));
  j = new Jzero();
    instructions.push(j)
    .push(new Load(addr_for_result))
    .push(new Sub(addr_for_result))
    .push(new Sub(addr_for_result))
    .push(new Store(addr_for_result))   // result *= -1
  .push(new NOP(j));
// }
  if (addr_for_result == data->get_register(Register::A)) {
    instructions
    .push(new Load(data->get_register(Register::A)))
    .push(new Storei(identifier_address));
  }

  return instructions;
}

InstructionVector TBinaryExpression::div(integer identifier_address) {
  InstructionVector instructions;
  
  if (NumberValue *lv = dynamic_cast<NumberValue*>(lvalue)) {
    if (NumberValue *rv = dynamic_cast<NumberValue*>(rvalue)) {
      if (rv->get_value() == 0) instructions.push(new Sub(0));
      else instructions.append(code->insert_to_acc(floor((double) lv->get_value() / (double) rv->get_value())));
      if (identifier_address == data->get_register(Register::IDR1)) {
        instructions.push(new Storei(identifier_address));
      } else {
        instructions.push(new Store(identifier_address));
      }
      return instructions;
    }
  }
  
  instructions.append(lvalue->store_in_register(Register::B))
  .append(rvalue->store_in_register(Register::C));
  TIdentifier *lid = new TVariableIdentifier(new Variable(data->get_register(Register::B)));
  TIdentifier *rid = new TVariableIdentifier(new Variable(data->get_register(Register::C)));

// if (divisor != 0) {
  instructions.append(rid->load());
  auto j = new Jzero();
    instructions.push(j)
    .append(code->set_sign_bit(lid, rid)) // stores sign bit in Register::D

    .push(new Sub(0));
    integer addr_for_result = identifier_address;
    if (identifier_address == data->get_register(Register::IDR1)) {
      addr_for_result = data->get_register(Register::A);
    }
    instructions.push(new Store(addr_for_result))   // result = 0
    .push(new Inc())
    .push(new Store(data->get_register(Register::E)))   // multiple = 1
  // while (scaled_divisor < dividend) {
    .append(lid->load())
    .push(new Store(data->get_register(Register::F)))   // remain = dividend
    .push(new Sub(rid->get_addr()));
    auto k = new Jneg();
    auto l = new Jzero();
    instructions.push(k)
    .push(l)                            // while scaled_divisor < dividend
      .append(rid->load())
      .append(code->lshift())
      .push(new Store(rid->get_addr()))                 // scaled_divisor *= 2
      
      .push(new Load(data->get_register(Register::E)))
      .append(code->lshift())
      .push(new Store(data->get_register(Register::E))) // multiple *= 2

      .append(lid->load())
      .push(new Sub(rid->get_addr()))
      .push(new Jump(k))
    .push(new NOP(l))
    .push(new NOP(k));
  // }
    // k = code->get_instruction_count();
  // do {
    // if (remain >= scaled_divisor) {
    auto ll = new Load(data->get_register(Register::F));
      instructions.push(ll)    // load remain 
      .push(new Sub(rid->get_addr()));                     // sub scaled_divisor
      auto jneg = new Jneg();
      instructions.push(jneg)
        .push(new Store(data->get_register(Register::F))) // remain -= scaled_divisor
        .push(new Load(addr_for_result))
        .push(new Add(data->get_register(Register::E)))
        .push(new Store(addr_for_result)) // result += multiple
      .push(new NOP(jneg))
      // }

      .push(new Load(rid->get_addr()))
      .append(code->rshift())
      .push(new Store(rid->get_addr()))                   // scaled_divisor >>= 1

      .push(new Load(data->get_register(Register::E)))
      .append(code->rshift())
      .push(new Store(data->get_register(Register::E)));   // multiple >>= 1

      auto end_while = new Jzero();
      instructions.push(end_while)
      .push(new Jump(ll))
    .push(new NOP(end_while))
  // } while (multiple != 0)
    
  // if (sign_bit) {
    .push(new Load(data->get_register(Register::D)));
    auto jzero = new Jzero();
    instructions.push(jzero)
      .push(new Load(addr_for_result))
      .push(new Sub(addr_for_result))
      .push(new Sub(addr_for_result))
      .push(new Store(addr_for_result))                 // negate result
      .push(new Load(data->get_register(Register::F)));
    // if (remain > 0) {
      auto _jzero = new Jzero();
      instructions.push(_jzero)
        .push(new Load(addr_for_result))
        .push(new Dec())                                                // result--
        .push(new Store(addr_for_result))
      .push(new NOP(_jzero))
    .push(new NOP(jzero));
    // }
    
    if (addr_for_result == data->get_register(Register::A)) {
      instructions
      .push(new Load(data->get_register(Register::A)))
      .push(new Storei(identifier_address));
    }
  
  // }
  auto z = new Jump();
  instructions.push(z)
  .push(new NOP(j));
// } else {
    if (addr_for_result == data->get_register(Register::A)) {
      instructions
      .push(new Load(data->get_register(Register::A)))
      .push(new Sub(0))
      .push(new Storei(identifier_address));
    } else {
      instructions
      .push(new Sub(0))
      .push(new Store(addr_for_result));
    }
  instructions.push(new NOP(z));
// }

  return instructions;
}

InstructionVector TBinaryExpression::mod(integer identifier_address) {
  InstructionVector instructions;

  if (NumberValue *lv = dynamic_cast<NumberValue*>(lvalue)) {
    if (NumberValue *rv = dynamic_cast<NumberValue*>(rvalue)) {
      if (rv->get_value() == 0) instructions.append(code->insert_to_acc(0));
      else instructions.append(code->insert_to_acc(((lv->get_value() % rv->get_value()) + rv->get_value()) % rv->get_value()));
      if (identifier_address == data->get_register(Register::IDR1)) {
        instructions
        .push(new Storei(identifier_address));
      } else {
        instructions
        .push(new Store(identifier_address));
      }
      return instructions;
    }
  }

  
  instructions.append(lvalue->store_in_register(Register::B))
  .append(rvalue->store_in_register(Register::C));
  TIdentifier *lid = new TVariableIdentifier(new Variable(data->get_register(Register::B)));
  TIdentifier *rid = new TVariableIdentifier(new Variable(data->get_register(Register::C)));

  integer addr_for_result = identifier_address;
  if (identifier_address == data->get_register(Register::IDR1)) {
    addr_for_result = data->get_register(Register::F);
  }

// if (divisor != 0) {
  instructions.append(rid->load());
  auto j = new Jzero();
    instructions.push(j)
    .append(code->set_sign_bit(lid, rid)) // stores sign bit in Register::D

    .push(new Sub(0))
    .push(new Store(data->get_register(Register::A)))   // result = 0
    .push(new Inc())
    .push(new Store(data->get_register(Register::E)))   // multiple = 1
  // while (scaled_divisor < dividend) {
    .append(lid->load())
    .push(new Store(addr_for_result))   // remain = dividend
    .push(new Sub(rid->get_addr()));
    auto k = new Jneg();
    auto l = new Jzero();
    instructions.push(k)
    .push(l)                            // while scaled_divisor < dividend
      .append(rid->load())
      .append(code->lshift())
      .push(new Store(rid->get_addr()))                 // scaled_divisor *= 2
      
      .push(new Load(data->get_register(Register::E)))
      .append(code->lshift())
      .push(new Store(data->get_register(Register::E))) // multiple *= 2

      .append(lid->load())
      .push(new Sub(rid->get_addr()))
      .push(new Jump(k))
    .push(new NOP(l))
    .push(new NOP(k));
  // }
  // do {
    // if (remain >= scaled_divisor) {
    auto load = new Load(addr_for_result);
      instructions.push(load)    // load remain 
      .push(new Sub(rid->get_addr()));                     // sub scaled_divisor
      auto jneg = new Jneg();
      instructions.push(jneg)
        .push(new Store(addr_for_result)) // remain -= scaled_divisor
        .push(new Load(data->get_register(Register::A)))
        .push(new Add(data->get_register(Register::E)))
        .push(new Store(data->get_register(Register::A))) // result += multiple
      .push(new NOP(jneg))
      // }

      .push(new Load(rid->get_addr()))
      .append(code->rshift())
      .push(new Store(rid->get_addr()))                   // scaled_divisor >>= 1

      .push(new Load(data->get_register(Register::E)))
      .append(code->rshift())
      .push(new Store(data->get_register(Register::E)));   // multiple >>= 1

      auto end_while = new Jzero();
      instructions.push(end_while)
      .push(new Jump(load))
    .push(new NOP(end_while))
  // } while (multiple != 0)
    
    .push(new Load(data->get_register(Register::D)));
  // if (sign_bit) {
    auto jzero = new Jzero();
    instructions.push(jzero)
      .append(rvalue->load_value());
      jneg = new Jneg(); instructions.push(jneg);
      auto jump = new Jump(); instructions.push(jump)
        .push(new NOP(jneg))
        .push(new Add(addr_for_result))
        .push(new Store(addr_for_result))
      .push(new NOP(jump))
      
      .append(lvalue->load_value());      
      jneg = new Jneg(); instructions.push(jneg);
      jump = new Jump(); instructions.push(jump)
      // code->jneg(2); code->jump(); // a < 0
        .push(new NOP(jneg))
        .append(rvalue->load_value())
        .push(new Sub(addr_for_result))
        .push(new Store(addr_for_result))
      .push(new NOP(jump));
      
      auto m = new Jump();
      instructions.push(m)
      // integer m = code->get_instruction_count();
      // code->jump();
    .push(new NOP(jzero))
  // } else { 
      .append(lvalue->load_value());
      auto jpos = new Jpos(); instructions.push(jpos)
        .push(new Load(addr_for_result))
        .push(new Sub(addr_for_result))
        .push(new Sub(addr_for_result))
        .push(new Store(addr_for_result))
      .push(new NOP(jpos))
    .push(new NOP(m));
  // }

    if (addr_for_result == data->get_register(Register::F)) {
      instructions
      .push(new Load(data->get_register(Register::F)))
      .push(new Storei(identifier_address));
    }

  auto z = new Jump(); instructions.push(z)
  .push(new NOP(j));
// } else {
    if (addr_for_result == data->get_register(Register::F)) {
      instructions
      .push(new Load(data->get_register(Register::F)))
      .push(new Sub(0))
      .push(new Storei(identifier_address));
    } else {
      instructions
      .push(new Sub(0))
      .push(new Store(addr_for_result));
    }
  instructions.push(new NOP(z));
// }

  return instructions;
}
