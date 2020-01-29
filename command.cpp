#include "types.hpp"
#include "code.hpp"

#include <iostream>

void TWhileCommand::load_command() {
  this->condition->load_condition();
  for (const auto &command : *this->while_block) {
    command->load_command();
  }
  code->jump(this->condition->get_cond_address());
  code->insert_jump_address(this->condition->get_jump_address());
}

void TDoWhileCommand::load_command() {
  integer do_jump = code->get_instruction_count();
  for (const auto &command : *this->do_while_block) {
    command->load_command();
  }
  this->condition->load_condition();
  code->jump(do_jump);
  code->insert_jump_address(this->condition->get_jump_address());
}

void TForToCommand::load_command(){
  Variable* it = data->new_iterator(this->pidentifier);

  end->load_value();
  code->store(it->get_addr() + 1); // it_end <- b
  start->load_value();
  code->store(it->get_addr());     // it     <- a

  integer s = code->get_instruction_count(); // s «═════ ╗
    code->sub(it->get_addr() + 1); // sub it_end         ║
    integer e = code->get_instruction_count();        // ║             
    code->jpos();                  // jpos e             ║
    for (const auto &command : *this->for_block) {    // ║
      command->load_command();     //                    ║
    }                              //                    ║
    code->load(it->get_addr());    // load it            ║
    code->inc();                   // inc                ║
    code->store(it->get_addr());   // store it           ║
    code->jump(s);                 // jump s  ═══════════╝
  code->insert_jump_address(e);    // e

  data->del_iterator(this->pidentifier);
}

void TForDownToCommand::load_command() {
  Variable* it = data->new_iterator(this->pidentifier);

  end->load_value();
  code->store(it->get_addr() + 1); // it_end <- b
  start->load_value();
  code->store(it->get_addr());     // it     <- a

  integer s = code->get_instruction_count(); // s «═════ ╗
    code->sub(it->get_addr() + 1); // sub it_end         ║
    integer e = code->get_instruction_count();        // ║             
    code->jneg();                  // jpos e             ║
    for (const auto &command : *this->for_block) {    // ║
      command->load_command();     //                    ║
    }                              //                    ║
    code->load(it->get_addr());    // load it            ║
    code->dec();                   // inc                ║
    code->store(it->get_addr());   // store it           ║
    code->jump(s);                 // jump s  ═══════════╝
  code->insert_jump_address(e);    // e

  data->del_iterator(this->pidentifier);
}

TIfCommand::TIfCommand(TCondition* condition, TCommandBlock* if_block) {
  this->condition = condition;
  this->if_block = if_block;
}

void TIfCommand::load_command() {
  this->condition->load_condition();
  for (const auto &command : *this->if_block) {
    command->load_command();
  }
  code->insert_jump_address(this->condition->get_jump_address());
}

TIfElseCommand::TIfElseCommand(TCondition* condition, TCommandBlock* if_block, TCommandBlock* else_block) : TIfCommand(condition, if_block) {
  this->else_block = else_block;
}

void TIfElseCommand::load_command() {
  this->condition->load_condition();
  for (const auto &command : *this->if_block) {
    command->load_command();
  }
  integer j = code->get_instruction_count();
  code->jump();
  code->insert_jump_address(this->condition->get_jump_address());
  for (const auto &command : *this->else_block) {
    command->load_command();
  }
  code->insert_jump_address(j);
}


TWriteCommand::TWriteCommand(TValue* value) {
  this->value = value;
}

void TWriteCommand::load_command() {
  code->write(this->value);
}


TReadCommand::TReadCommand(TIdentifier* identifier) {
  this->identifier = identifier;
}

void TReadCommand::load_command() {
  code->read(this->identifier);
}


TAssignCommand::TAssignCommand(TIdentifier* identifier, TExpression* expression) {
  this->identifier = identifier;
  this->expression = expression;
}

void TAssignCommand::load_command() {
  code->assign(this->identifier, this->expression);
}


