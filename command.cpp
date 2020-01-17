#include "types.hpp"
#include "code.hpp"

#include <iostream>

TCommandBlock::TCommandBlock(TCommand* command) {
  this->add_command(command);
}

void TCommandBlock::add_command(TCommand* command) {
  this->commands.push_back(command);
}

void TCommandBlock::load_commands() {
  for (const auto &command : this->commands) {
    command->load_command();
  }
}

void TWhileCommand::load_command() {
  condition->load_condition();
  while_block->load_commands();
  code->jump(condition->get_cond_address());
  code->insert_jump_address(condition->get_jump_address());
}

void TDoWhileCommand::load_command() {
  integer do_jump = code->get_instruction_count();
  do_while_block->load_commands();
  condition->load_condition();
  code->jump(do_jump);
  code->insert_jump_address(condition->get_jump_address());
}

void TForToCommand::load_command(){
  Variable* it = data->new_iterator(this->pidentifier);

  if (auto s = dynamic_cast<NumberValue*>(start)) {
    if (auto e = dynamic_cast<NumberValue*>(end)) {
      integer iterations = e->get_value() - s->get_value() + 1;

      start->load_value();
      code->store(it->get_addr());
      for (int i = 1; i < iterations; i++) {
        code->inc();
        code->store(it->get_addr() + i);
        data->inc_iterator_count();
      }

      for (int i = 0; i < iterations; i++) {
        for_block->load_commands();
        it->set_addr(it->get_addr() + 1);
      }

      return;
    }
  }
  end->load_value();
  code->store(it->get_addr() + 1); // it_end <- b
  start->load_value();
  code->store(it->get_addr());     // it     <- a

  integer s = code->get_instruction_count(); // s «════╗
    code->sub(it->get_addr() + 1); // sub it_end        ║
    integer e = code->jpos();      // jpos e            ║             
    for_block->load_commands();    //                   ║
    code->load(it->get_addr());    // load it           ║
    code->inc();                   // inc               ║
    code->store(it->get_addr());   // store it          ║
    code->jump(s);                 // jump s  ════════╝
  code->insert_jump_address(e);    // e

  data->del_iterator(this->pidentifier);
  
}

void TForDownToCommand::load_command() {
  Variable* it = data->new_iterator(this->pidentifier);

  if (auto s = dynamic_cast<NumberValue*>(start)) {
    if (auto e = dynamic_cast<NumberValue*>(end)) {
      integer iterations = s->get_value() - e->get_value() + 1;

      start->load_value();
      code->store(it->get_addr());     // it     <- a
      for (int i = 1; i < iterations; i++) {
        code->dec();
        code->store(it->get_addr() + i);
        data->inc_iterator_count();
      }

      for (int i = 0; i < iterations; i++) {
        for_block->load_commands();
        it->set_addr(it->get_addr() + 1);
      }

      return;
    }
  }
  end->load_value();
  code->store(it->get_addr() + 1); // it_end <- b
  start->load_value();
  code->store(it->get_addr());     // it     <- a

  integer s = code->get_instruction_count(); // s «════╗
    code->sub(it->get_addr() + 1); // sub it_end        ║
    integer e = code->jneg();      // jpos e            ║
    for_block->load_commands();    //                   ║
    code->load(it->get_addr());    // load it           ║
    code->dec();                   // inc               ║
    code->store(it->get_addr());   // store it          ║
    code->jump(s);                 // jump s   ═══════╝
  code->insert_jump_address(e);    // e

  data->del_iterator(this->pidentifier);
}

void TIfCommand::load_command() {
  condition->load_condition();
  if_block->load_commands();
  code->insert_jump_address(this->condition->get_jump_address());
}

void TIfElseCommand::load_command() {
  condition->load_condition();
  if_block->load_commands();
  integer j = code->jump();
  code->insert_jump_address(condition->get_jump_address());
  else_block->load_commands();
  code->insert_jump_address(j);
}

void TWriteCommand::load_command() {
  this->value->load_value(); // ACC = value.value
  code->put();          // PUT
}

void TReadCommand::load_command() {
  if (TArrayVariableIdentifier *id = dynamic_cast<TArrayVariableIdentifier*>(this->identifier)) {
    id->load_addr_to_register(Register::IDR);         // IDR = id.addr
    code->get();                    // ACC = input
    code->storei(data->get_register(Register::IDR));  // p(IDR) = ACC
  } else {
    code->get();                         // ACC = input
    code->store(this->identifier->get_addr()); // p(id.addr) = ACC
  }
}

void TAssignCommand::load_command() {
  this->expression->load_expr(this->identifier);
}
