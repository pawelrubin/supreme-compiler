#pragma once

#include "typedefs.hpp"
#include "data.hpp"

class Code;

extern Code *code;
extern Data *data;

class TProgram;
// class TDeclaration;
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

typedef std::vector<TCommand*> TCommandBlock;
// typedef std::vector<TDeclaration*> TDeclarationBlock;

class TProgram {
  protected:
    TCommandBlock* commands;

  public:
    TProgram(TCommandBlock*);
    virtual void load_program();
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


class TCommand {
  protected:
    integer length;
  public:
    TCommand() = default;
    virtual void load_command() {}
};

class TIfCommand : public TCommand {
  protected:
    TCondition* condition;
    TCommandBlock* if_block;
    
    integer jump_address;

  public:
    TIfCommand(TCondition*, TCommandBlock*);
    void load_command() override;
};

class TIfElseCommand : public TIfCommand {
  private:
    TCommandBlock* else_block;

  public:
    TIfElseCommand(TCondition*, TCommandBlock*, TCommandBlock*);
};

class TWriteCommand : public TCommand {
  private:
    TValue* value;

  public:
    TWriteCommand(TValue*);
    void load_command() override;
};

class TReadCommand : public TCommand {
  private:
    TIdentifier* identifier;

  public:
    TReadCommand(TIdentifier*);
    void load_command() override;
};

class TAssignCommand : public TCommand {
  private:
    TIdentifier* identifier;
    TExpression* expression;

  public:
    TAssignCommand(TIdentifier*, TExpression*);
    void load_command() override;
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
    integer address;

    void eq();
    void neq();
    void le();
    void ge();
    void leq();
    void geq();

  public:
    integer get_address();
    void load_condition();
    TCondition(TValue*, TValue*, ConditionOperator);
};

/*
 ********************
 *    EXPRESSION    *
 ********************
 */

class TExpression {
  public:
    virtual void load_expr() {};
};

class TValueExpression : public TExpression {
  private:
    TValue *value;

  public:
    TValueExpression(TValue*);
    void load_expr() override;
};

class TBinaryExpression : public TExpression {
  private:
    TValue *lvalue;
    TValue *rvalue;
    BinaryOperator op;
    void plus();
    void minus();
    void times();
    void div();
    void mod();

  public:
    TBinaryExpression(TValue*, TValue*, BinaryOperator);
    void load_expr() override;
};



/*
 ***************
 *    VALUE    *
 ***************
 */

class TValue {
  protected:
    integer value;

  public:
    integer get_value();
    virtual void load_value() {}; // loads variable value to ACC
    virtual void insert_to_VLR() {}; // loads variable value to VLR
    virtual void store_in_register(Register) {};
};

class NumberValue : public TValue {
  public:
    NumberValue(integer);
    void load_value() override;
    void insert_to_VLR() override;
    void store_in_register(Register) override;
};

class IdentifierValue : public TValue {
  private:
    TIdentifier *identifier;

  public:
    IdentifierValue(TIdentifier*);
    void load_value() override;
    void insert_to_VLR() override;
    void store_in_register(Register) override;
    TIdentifier* get_identifier();
};


/*
 ********************
 *    IDENTIFIER    *
 ********************
 */

class TIdentifier {
  protected:
  // TODO: write costs of operations in comments
  public:
    virtual void load_addr_to_register(Register) {} // stores identifier address in IDR
    virtual void load_value_to_register(Register); // stores identifier value in IDR
    virtual void load_value_to_acc() {}
    virtual integer get_addr() { return 0; }
    virtual void negate(bool) {}
};

class TVariableIdentifier : public TIdentifier {
  private:
    Variable *variable;

  public:
    TVariableIdentifier(ident);
    TVariableIdentifier(Variable*);
    void load_addr_to_register(Register) override;
    void load_value_to_acc() override;
    integer get_addr();
    void negate(bool) override;
};

class TArrayVariableIdentifier : public TIdentifier {
  private:
    Array *array;
    Variable *variable;

  public:
    using TIdentifier::get_addr;
    TArrayVariableIdentifier(ident, ident);
    void load_addr_to_register(Register) override;
    void load_value_to_acc() override;
    void negate(bool) override;
};

class TArrayNumIdentifier : public TIdentifier {
  private:
    Array *array;
    integer num_value;

  public:
    TArrayNumIdentifier(ident, integer);
    void load_addr_to_register(Register) override;
    void load_value_to_acc() override;
    integer get_addr();
    void negate(bool) override;
};

