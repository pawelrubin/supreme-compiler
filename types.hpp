#pragma once

#include "typedefs.hpp"
#include "data.hpp"
#include "instruction.hpp"

class Code;

extern Code *code;
extern Data *data;

class TProgram;
// class TDeclaration;
class TCommandBlock;
class TCommand;
class TCondition;
class TExpression;
class TValue;
class TIdentifier;


enum class BinaryOperator {
  PLUS,
  MINUS,
  TIMES,
  DIV,
  MOD
};

enum class ConditionOperator {
  EQ,
  NEQ,
  LE,
  GE,
  LEQ,
  GEQ
};

typedef std::vector<TCommand*> commandList;
// typedef std::vector<TDeclaration*> TDeclarationBlock;

class TProgram {
  protected:
    TCommandBlock* commands;

  public:
    TProgram(TCommandBlock* cb) : commands(cb) {}
    virtual InstructionVector load_program();
};

// class TProgramWithDeclarations : public TProgram {
//   private:
//     TDeclarationBlock* declarations;

//   public:
//     TProgramWithDeclarations(TDeclarationBlock*, TCommandBlock*);
//     void load_program() override;
// };

// class TDeclaration {
//   protected:
//     ident pidentifier;
//   public:
//     TDeclaration(ident);
//     virtual void load_declaration();
// };

// class TArrayDeclaration : public TDeclaration {
//   private:
//     integer start;
//     integer end;

//   public:
//     TArrayDeclaration(ident, integer, integer);
//     void load_declaration() override;
// };

/*
 *****************
 *    COMMAND    *
 *****************
 */

class TCommandBlock {
  private:
    commandList commands;

  public:
    TCommandBlock(TCommand*); 
    void add_command(TCommand*);
    InstructionVector load_commands();
};

class TCommand {
  public:
    TCommand() = default;
    virtual InstructionVector get_instructions() {}
};

class TAssignCommand : public TCommand {
  private:
    TIdentifier* identifier;
    TExpression* expression;

  public:
    TAssignCommand(TIdentifier* id, TExpression* ex)
      : identifier(id), expression(ex) {}
    InstructionVector get_instructions() override;
};

class TIfCommand : public TCommand {
  protected:
    TCondition* condition;
    TCommandBlock* if_block;
    
  public:
    TIfCommand(TCondition* c, TCommandBlock* ib)
     : condition(c), if_block(ib) {} 
    InstructionVector get_instructions() override;
};

class TIfElseCommand : public TIfCommand {
  private:
    TCommandBlock* else_block;

  public:
    TIfElseCommand(TCondition* c, TCommandBlock* ib, TCommandBlock* eb)
      : TIfCommand(c, ib), else_block(eb) {} 
    InstructionVector get_instructions() override;
};

class TWhileCommand : public TCommand {
  private:
    TCondition* condition;
    TCommandBlock* while_block;

  public:
    TWhileCommand(TCondition* c, TCommandBlock* b) 
      : condition(c), while_block(b) {}
    InstructionVector get_instructions() override;
};

class TDoWhileCommand : public TCommand {
  private:
    TCommandBlock* do_while_block;
    TCondition* condition;

  public:
    TDoWhileCommand(TCommandBlock* b, TCondition* c)
      : do_while_block(b), condition(c) {}
    InstructionVector get_instructions() override;
};

class TForCommand : public TCommand {
  protected:
    ident pidentifier;
    TValue* start;
    TValue* end;
    TCommandBlock* for_block;

  public:
    TForCommand(ident p, TValue* s, TValue* e, TCommandBlock* c)
      : pidentifier(p), start(s), end(e), for_block(c) {}
};

class TForToCommand : public TForCommand {
  public:
    using TForCommand::TForCommand;
    InstructionVector get_instructions() override;
};

class TForDownToCommand : public TForCommand {
  public:
    using TForCommand::TForCommand;
    InstructionVector get_instructions() override;
};

class TReadCommand : public TCommand {
  private:
    TIdentifier* identifier;

  public:
    TReadCommand(TIdentifier* id) : identifier(id) {}
    InstructionVector get_instructions() override;
};

class TWriteCommand : public TCommand {
  private:
    TValue* value;

  public:
    TWriteCommand(TValue* v) : value(v) {}
    InstructionVector get_instructions() override;
};

/*
 *******************
 *    CONDITION    *
 *******************
 */

class TCondition {
  private:
    TValue *lvalue;
    TValue *rvalue;
    ConditionOperator op;
    // integer jump_address;
    JumpInstruction* jump;
    integer cond_address;

    InstructionVector eq();
    InstructionVector neq();
    InstructionVector le();
    InstructionVector ge();
    InstructionVector leq();
    InstructionVector geq();

  public:
    // integer get_jump_address();
    JumpInstruction* get_jump();
    integer get_cond_address();
    InstructionVector load_condition();
    TCondition(TValue* lv, TValue* rv, ConditionOperator o)
      : lvalue(lv), rvalue(rv), op(o) {}
};

/*
 ********************
 *    EXPRESSION    *
 ********************
 */

class TExpression {
  public:
    virtual InstructionVector load_expr(TIdentifier*) {};
};

class TValueExpression : public TExpression {
  private:
    TValue *value;

  public:
    TValueExpression(TValue* v) : value(v) {}
    InstructionVector load_expr(TIdentifier*) override;
};

class TBinaryExpression : public TExpression {
  private:
    TValue *lvalue;
    TValue *rvalue;
    BinaryOperator op;
    InstructionVector plus();
    InstructionVector minus();
    InstructionVector times(integer addr_for_result);
    InstructionVector div(integer addr_for_result);
    InstructionVector mod(integer addr_for_result);

  public:
    TBinaryExpression(TValue* lv, TValue* rv, BinaryOperator o)
      : lvalue(lv), rvalue(rv), op(o) {}
    InstructionVector load_expr(TIdentifier*) override;
};



/*
 ***************
 *    VALUE    *
 ***************
 */

class TValue {
  public:
    virtual InstructionVector load_value() {}; // loads variable value to ACC
    virtual InstructionVector insert_to_VLR() {}; // loads variable value to VLR
    virtual InstructionVector store_in_register(Register) {};
};

class NumberValue : public TValue {
  private:
    integer value;

  public:
    NumberValue(integer v) : value(v) {}
    InstructionVector load_value() override;
    InstructionVector insert_to_VLR() override;
    InstructionVector store_in_register(Register) override;
    integer get_value();
};

class IdentifierValue : public TValue {
  private:
    TIdentifier *identifier;

  public:
    IdentifierValue(TIdentifier*);
    InstructionVector load_value() override;
    InstructionVector insert_to_VLR() override; // TODO: delete this lmao
    InstructionVector store_in_register(Register) override;
    TIdentifier* get_identifier();
};

/*
 ********************
 *    IDENTIFIER    *
 ********************
 */

class TIdentifier {
  protected:
    ident name;
  // TODO: write costs of operations in comments
  public:
    TIdentifier() = default;
    TIdentifier(ident n) : name(n) {}
    virtual InstructionVector load_addr_to_register(Register) {} // stores identifier jump_address in IDR
    virtual InstructionVector load_value_to_register(Register);  // stores identifier value in IDR
    virtual InstructionVector load() {}
    virtual InstructionVector negate() {}
    virtual integer get_addr() { return 0; }
    virtual ident get_name() { return name; }
};

class TVariableIdentifier : public TIdentifier {
  private:
    Variable *variable;

  public:
    TVariableIdentifier(ident);
    TVariableIdentifier(Variable* v) : variable(v) {}
    InstructionVector load_addr_to_register(Register) override;
    InstructionVector load() override;
    integer get_addr();
    InstructionVector negate() override;
};

class TArrayVariableIdentifier : public TIdentifier {
  private:
    Array *array;
    Variable *variable;

  public:
    using TIdentifier::get_addr;
    TArrayVariableIdentifier(ident, ident);
    InstructionVector load_addr_to_register(Register) override;
    InstructionVector load() override;
    InstructionVector negate() override;
};

class TArrayNumIdentifier : public TIdentifier {
  private:
    Array *array;
    integer num_value;

  public:
    TArrayNumIdentifier(ident, integer);
    InstructionVector load_addr_to_register(Register) override;
    InstructionVector load() override;
    integer get_addr();
    InstructionVector negate() override;
};
