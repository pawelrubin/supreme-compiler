#pragma once

#include "typedefs.hpp"
#include "data.hpp"

extern Data *data;

class TExpression {};

// Identifier
class TIdentifier {
  protected:
    integer mem_addr;

  public:
    integer get_mem_addr();
};

class TVariableIdentifier : public TIdentifier {
  public:
    TVariableIdentifier(ident var_name);
};

class TArrayVariableIdentifier : public TIdentifier {
  public:
    TArrayVariableIdentifier(ident array_name, ident index_name);
};

class TArrayNumIdentifier : public TIdentifier {
  public:
    TArrayNumIdentifier(ident array_name, integer num_value);
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

