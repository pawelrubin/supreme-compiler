#include "types.hpp"
#include "code.hpp"

TIfCommand::TIfCommand(TCondition* condition, TCommandBlock* if_block) {
  this->condition = condition;
  this->if_block = if_block;
  // this->load_command();
}

void TIfCommand::load_command() {
  this->condition->load_condition();
  for (const auto &command : *this->if_block) {
    command->load_command();
  }
  // TCommandBlock::iterator iter,end;
  // for ( iter = if_block->begin(), end = if_block->end(); iter != end; ++iter) {
  //   // (*iter)->load_command();
  // }
  code->insert_jump_address(this->condition->get_address(), code->get_instruction_count());
}

TIfElseCommand::TIfElseCommand(TCondition* condition, TCommandBlock* if_block, TCommandBlock* else_block) : TIfCommand(condition, if_block) {
  this->else_block = else_block;
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


