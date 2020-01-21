#include "types.hpp"
#include "code.hpp"

JumpInstruction* TCondition::get_jump() {
  return this->jump;
}

InstructionVectorWithJump TCondition::load_condition() {
  InstructionVectorWithJump instructions;
  
  instructions.append(
    TBinaryExpression(lvalue, rvalue, BinaryOperator::MINUS)
    .load_expr(new TVariableIdentifier(new Variable(0)))
  );

  switch (this->op) {
  case ConditionOperator::EQ:
    instructions.append(this->eq());
    break;
  case ConditionOperator::NEQ:
    instructions.append(this->neq());
    break;
  case ConditionOperator::LE:
    instructions.append(this->le());
    break;
  case ConditionOperator::GE:
    instructions.append(this->ge());
    break;
  case ConditionOperator::LEQ:
    instructions.append(this->leq());
    break;
  case ConditionOperator::GEQ:
    instructions.append(this->geq());
    break;
  }

  return instructions;
}

InstructionVectorWithJump TCondition::eq() {
  InstructionVectorWithJump code;
  auto jzero = new Jzero();
  auto jump = new Jump();
  auto nop = new NOP(jzero);
  code.set_jump(jump);
  
  code.push(jzero)
  .push(jump)
  .push(nop);

  return code;
}

InstructionVectorWithJump TCondition::neq() {
  InstructionVectorWithJump code;
  auto jzero = new Jzero();
  code.set_jump(jzero);
  
  code.push(jzero);

  return code;
}

InstructionVectorWithJump TCondition::le() {
  InstructionVectorWithJump code;
  auto jneg = new Jneg();
  auto jump = new Jump();
  auto nop = new NOP(jneg);
  code.set_jump(jump);
  
  code.push(jneg)
  .push(jump)
  .push(nop);

  return code;
}

InstructionVectorWithJump TCondition::ge() {
  InstructionVectorWithJump code;
  auto jpos = new Jpos();
  auto jump = new Jump();
  auto nop = new NOP(jpos);
  code.set_jump(jump);
  
  code.push(jpos)
  .push(jump)
  .push(nop);

  return code;
}

InstructionVectorWithJump TCondition::leq() {
  InstructionVectorWithJump code;
  auto jpos = new Jpos();
  code.set_jump(jpos);
  
  code.push(jpos);

  return code;
}

InstructionVectorWithJump TCondition::geq() {
  InstructionVectorWithJump code;
  auto jneg = new Jneg();
  code.set_jump(jneg);
  
  code.push(jneg);

  return code;
}
