#pragma once

#include "typedefs.hpp"
#include "data.hpp"

class Code;

extern Code *code;
extern Data *data;

class TExpresion;
class TValue;
class TIdentifier;

class TExpression {

  public:
    TExpression() = default;
    void load_expr();
};

class TValueExpression : public TExpression {
  private:
    TValue *value;

  public:
    TValueExpression(TValue *value);
    void load_expr() ;
};



// Value
class TValue {
  protected:
    integer value;

  public:
    integer get_value();
};

class NumberValue : public TValue {
  public:
    NumberValue(integer value);
};

class IdentifierValue : public TValue {
  private:
    TIdentifier *identifier;

  public:
    IdentifierValue(TIdentifier *identifier);
};


// Identifier
class TIdentifier {
  protected:
    integer mem_addr;

  public:
    integer get_mem_addr();
    virtual void load_identifier_to_reg() {};
};

class TVariableIdentifier : public TIdentifier {
  private:
    Variable *variable;

  public:
    TVariableIdentifier(ident var_name);
    void load_identifier_to_reg() override; // stores identifier address in identifier register
};

class TArrayVariableIdentifier : public TIdentifier {
  public:
    TArrayVariableIdentifier(ident array_name, ident index_name);
    // void load_identifier_to_reg() override; 
};

class TArrayNumIdentifier : public TIdentifier {
  public:
    TArrayNumIdentifier(ident array_name, integer num_value);
    // void load_identifier_to_reg() override; 
};

