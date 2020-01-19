#include "instruction.hpp"
#include "data.hpp"
#include "code.hpp"

#include <iostream>
#include <vector>

Data *data = new Data();

InstructionVector init() {
  InstructionVector code; code
  .push(new Sub(0))
  .push(new Dec())
  .push(new Store(2))
  .push(new Inc())
  .push(new Inc())
  .push(new Store(1));
  return code;
}

InstructionVectorWithJump le() {
  InstructionVectorWithJump code;
  // auto jneg = new Jneg();
  auto jump = new Jump();
  auto nop = new NOP(jump);
  code.set_jump(jump);
  
  code.push(new Jneg(nop))
  .push(jump)
  .push(nop);

  return code;
}

int main() {
  InstructionVector instructions;
  
  instructions
  .push(new Sub(0))
  .push(new Store(1))
  .append(init()).push(new Get())
  .push(new Sub(1));
  auto lec = le();
  auto lec_jump = lec.get_jump();
  instructions.append(lec)
    .push(new Sub(0))
    .push(new Inc())
    .push(new Store(Register::A));
    auto k = new Jump();
  instructions.push(k)
    .push(new Sub(0))
    .push(new Dec())
    .push(new Store(Register::A));
  auto l = new Load(Register::A); 
  static_cast<JumpInstruction*>(lec_jump)->set_jump_destination(l); k->set_jump_destination(l);
  instructions.push(l)
  .push(new Put())
  .push(new Halt());

  CodeGenerator code_generator(instructions);
  code_generator.peephole();
  code_generator.generateCode();

  std::cerr << instructions.begin()->get_pc();
}

