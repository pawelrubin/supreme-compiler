#include "types.hpp"
#include "code.hpp"
#include "instruction.hpp"

#include <iostream>

TCommandBlock::TCommandBlock(TCommand* command) {
  this->add_command(command);
}

void TCommandBlock::add_command(TCommand* command) {
  this->commands.push_back(command);
}

InstructionVector TCommandBlock::load_commands() {
  InstructionVector instructions;
  for (const auto &command : this->commands) {
    instructions.append(command->get_instructions());
  }
  return instructions;
}

InstructionVector TWhileCommand::get_instructions() {
  InstructionVector instructions;
  auto c = condition->load_condition(); instructions
  .append(c)
    .append(while_block->load_commands())
    .push(new Jump(c.begin()))
  .push(new NOP(condition->get_jump()));
  return instructions;
}

InstructionVector TDoWhileCommand::get_instructions() {
  InstructionVector instructions;

  auto c = do_while_block->load_commands();
  instructions
  .append(c)
  .append(condition->load_condition())
  .push(new Jump(c.begin()))
  .push(new NOP(condition->get_jump()));

  return instructions;
}

InstructionVector TForToCommand::get_instructions(){
  Variable* it = data->new_iterator(this->pidentifier);
  InstructionVector instructions;
  if (auto s = dynamic_cast<NumberValue*>(start)) {
    if (auto e = dynamic_cast<NumberValue*>(end)) {
      integer iterations = e->get_value() - s->get_value() + 1;

      instructions
      .append(start->load_value())
      .push(new Store(it->get_addr()));
      for (int i = 1; i < iterations; i++) {
        instructions
        .push(new Inc())
        .push(new Store(it->get_addr() + i));
        data->inc_iterator_count();
      }

      for (int i = 0; i < iterations; i++) {
        instructions.append(for_block->load_commands());
        it->set_addr(it->get_addr() + 1);
      }

      return instructions;
    }
  }
  
  instructions
  .append(end->load_value())
  .push(new Store(it->get_addr() + 1)) // it_end <- b
  .append(start->load_value())
  .push(new Store(it->get_addr()));    // it     <- a

  auto s = new Sub(it->get_addr() + 1); instructions.push(s); // s: sub it_end s «════╗
  auto e = new Jpos(); instructions.push(e)                   // jpos e                ║
    .append(for_block->load_commands())                       //                       ║
    .push(new Load(it->get_addr()))                           // load it               ║
    .push(new Inc())                                          // inc                   ║
    .push(new Store(it->get_addr()))                          // store it              ║
    .push(new Jump(s))                                        // jump s  ════════════╝
  .push(new NOP(e));
  
  data->del_iterator(this->pidentifier);
  
  return instructions;
}

InstructionVector TForDownToCommand::get_instructions() {
  Variable* it = data->new_iterator(this->pidentifier);
  InstructionVector instructions;
  if (auto s = dynamic_cast<NumberValue*>(start)) {
    if (auto e = dynamic_cast<NumberValue*>(end)) {
      integer iterations = s->get_value() - e->get_value() + 1;

      instructions
      .append(start->load_value())
      .push(new Store(it->get_addr()));
      for (int i = 1; i < iterations; i++) {
        instructions
        .push(new Dec())
        .push(new Store(it->get_addr() + i));
        data->inc_iterator_count();
      }

      for (int i = 0; i < iterations; i++) {
        instructions.append(for_block->load_commands());
        it->set_addr(it->get_addr() + 1);
      }

      return instructions;
    }
  }
  
  instructions
  .append(end->load_value())
  .push(new Store(it->get_addr() + 1)) // it_end <- b
  .append(start->load_value())
  .push(new Store(it->get_addr()));    // it     <- a

  auto s = new Sub(it->get_addr() + 1); instructions.push(s); // s: sub it_end s «════╗
  auto e = new Jneg(); instructions.push(e)                   // jpos e                ║
    .append(for_block->load_commands())                       //                       ║
    .push(new Load(it->get_addr()))                           // load it               ║
    .push(new Dec())                                          // inc                   ║
    .push(new Store(it->get_addr()))                          // store it              ║
    .push(new Jump(s))                                        // jump s  ════════════╝
  .push(new NOP(e));
  
  data->del_iterator(this->pidentifier);
  
  return instructions;
}

InstructionVector TIfCommand::get_instructions() {
  InstructionVector instructions;
  auto c = condition->load_condition(); 
    instructions.append(c)
    .append(if_block->load_commands())
  .push(new NOP(c.get_jump()));
  return instructions;
}

InstructionVector TIfElseCommand::get_instructions() {
  InstructionVector instructions;
  auto c = condition->load_condition();
    instructions.append(c)
    .append(if_block->load_commands());
    auto end_if = new Jump();
  instructions.push(end_if);
    auto e = else_block->load_commands();
    c.get_jump()->set_jump_destination(e.begin());
    instructions.append(e)
  .push(new NOP(end_if));
  return instructions;
}

InstructionVector TWriteCommand::get_instructions() {
  InstructionVector instructions;
  instructions
  .append(this->value->load_value()) // ACC = value.value
  .push(new Put());                  // PUT
  return instructions;
}

InstructionVector TReadCommand::get_instructions() {
  InstructionVector instructions;
  if (TArrayVariableIdentifier *id = dynamic_cast<TArrayVariableIdentifier*>(this->identifier)) {
    instructions
    .append(id->load_addr_to_register(Register::IDR)) // IDR = id.addr
    .push(new Get())                                  // ACC = input
    .push(new Storei(Register::IDR));                 // p(IDR) = ACC
  } else {
    instructions
    .push(new Get())                                // ACC = input
    .push(new Store(this->identifier->get_addr())); // p(id.addr) = ACC
  }
  return instructions;
}

InstructionVector TAssignCommand::get_instructions() {
  return this->expression->load_expr(this->identifier);
}
