#include "types.hpp"
#include "code.hpp"

JumpInstruction* TCondition::get_jump() {
  return this->jump;
}

integer TCondition::get_cond_address() {
  return this->cond_address;
}

InstructionVector TCondition::load_condition() {
  InstructionVector instructions;
  
  instructions.append(
    TBinaryExpression(lvalue, rvalue, BinaryOperator::MINUS).load_expr(new TVariableIdentifier(new Variable(0)))
  );

  switch (this->op) {
  case ConditionOperator::EQ:
      
    code->jzero(2);
    code->jump();
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
  }
}

InstructionVector TCondition::eq() {
  code->jzero(2);
  code->jump();
  this->jump_address++;
}

InstructionVector TCondition::neq() {
  code->jzero();
}

InstructionVector TCondition::le() {
  code->jneg(2);
  code->jump();
  this->jump_address++;
}

InstructionVector TCondition::ge() {
  code->jpos(2);
  code->jump();
  this->jump_address++;
}

InstructionVector TCondition::leq() {
  code->jpos();
}

InstructionVector TCondition::geq() {
  code->jneg();
}
