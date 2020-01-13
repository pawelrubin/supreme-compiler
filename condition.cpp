#include "types.hpp"
#include "code.hpp"

TCondition::TCondition(TValue* lvalue, TValue* rvalue, ConditionOperator op) {
  this->lvalue = lvalue;
  this->rvalue = rvalue;
  this->op = op;
}

integer TCondition::get_jump_address() {
  return this->jump_address;
}

integer TCondition::get_cond_address() {
  return this->cond_address;
}

void TCondition::load_condition() {
  cond_address = code->get_instruction_count();
  TBinaryExpression(lvalue, rvalue, BinaryOperator::MINUS).load_expr(new TVariableIdentifier(new Variable(0)));
  jump_address = code->get_instruction_count();
  switch (this->op) {
  case ConditionOperator::EQ:
    this->eq();
    break;
  case ConditionOperator::NEQ:
    this->neq();
    break;
  case ConditionOperator::LE:
    this->le();
    break;
  case ConditionOperator::GE:
    this->ge();
    break;
  case ConditionOperator::LEQ:
    this->leq();
    break;
  case ConditionOperator::GEQ:
    this->geq();
    break;
  default:
    break;
  }
}

void TCondition::eq() {
  code->jzero(2);
  code->jump();
  this->jump_address++;
}

void TCondition::neq() {
  code->jzero();
}

void TCondition::le() {
  code->jneg(2);
  code->jump();
  this->jump_address++;
}

void TCondition::ge() {
  code->jpos(2);
  code->jump();
  this->jump_address++;
}

void TCondition::leq() {
  code->jpos();
}

void TCondition::geq() {
  code->jneg();
}
