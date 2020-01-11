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
    virtual void load_value() {}; // loads variable value to ACC
    virtual void insert_to_VLR() {}; // loads variable value to VLR
};

class NumberValue : public TValue {
  public:
    NumberValue(integer value);
    void load_value() override;
    void insert_to_VLR() override;
};

class IdentifierValue : public TValue {
  private:
    TIdentifier *identifier;

  public:
    IdentifierValue(TIdentifier *identifier);
    void load_value() override;
    void insert_to_VLR() override;
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
    virtual void load_addr_to_idr(int id=0) {}; // stores identifier address in IDR
    virtual integer get_addr() { return 0; }
    virtual void load_value_to_acc() {}
    virtual void load_value_to_idr() {}
};

class TVariableIdentifier : public TIdentifier {
  private:
    Variable *variable;

  public:
    TVariableIdentifier(ident var_name);
    void load_addr_to_idr(int id) override;
    void load_value_to_acc() override;
    void load_value_to_idr() override;
    integer get_addr();
};

class TArrayVariableIdentifier : public TIdentifier {
  private:
    Array *array;
    Variable *variable;

  public:
    using TIdentifier::get_addr;
    TArrayVariableIdentifier(ident array_name, ident index_name);
    void load_addr_to_idr(int id=0) override; 
    void load_value_to_acc() override;
    void load_value_to_idr() override;
};

class TArrayNumIdentifier : public TIdentifier {
  private:
    Array *array;
    integer num_value;

  public:
    TArrayNumIdentifier(ident array_name, integer num_value);
    void load_addr_to_idr(int id) override;
    void load_value_to_acc() override;
    void load_value_to_idr() override;
    integer get_addr();
};

