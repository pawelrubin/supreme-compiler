#pragma once

#include "typedefs.hpp"
#include "data.hpp"

class Code;

extern Code *code;
extern Data *data;

class TExpresion;
class TValue;
class TIdentifier;


enum class BinaryOperator {
  PLUS,
  MINUS,
  TIMES,
  DIV,
  MOD
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
    TValueExpression(TValue *value);
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
    TBinaryExpression(TValue *lvalue, TValue *rvalue, BinaryOperator op);
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
    virtual void load_value() {}; // laods variable value to ACC
};

class NumberValue : public TValue {
  public:
    NumberValue(integer value);
    void load_value() override;
};

class IdentifierValue : public TValue {
  private:
    TIdentifier *identifier;

  public:
    IdentifierValue(TIdentifier *identifier);
    void load_value() override;
};


/*
 ********************
 *    IDENTIFIER    *
 ********************
 */

class TIdentifier {
  protected:

  public:
    virtual void load_addr_to_idr() {}; // stores identifier address in IDR
};

class TVariableIdentifier : public TIdentifier {
  private:
    Variable *variable;

  public:
    TVariableIdentifier(ident var_name);
    void load_addr_to_idr() override;
};

class TArrayVariableIdentifier : public TIdentifier {
  private:
    Array *array;
    Variable *variable;

  public:
    TArrayVariableIdentifier(ident array_name, ident index_name);
    void load_addr_to_idr() override; 
};

class TArrayNumIdentifier : public TIdentifier {
  private:
    Array *array;
    integer num_value;

  public:
    TArrayNumIdentifier(ident array_name, integer num_value);
    void load_addr_to_idr() override; 
};

