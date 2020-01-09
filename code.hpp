#pragma once

#include "data.hpp"
#include "typedefs.hpp"
#include "types.hpp"

#include <vector>
#include <string>


class Code {
  private:
    codeList code;
    long long code_offset = 0;
    void update_offset(long long value);

  public:
    void start_code();
    void end_code();
    codeList get_code();
    void print_code();

    void assign(TIdentifier *identifier, TExpression *expr);

    void inc();
    void dec();
    void store(integer i);
    void load(integer i);

    void reset_acc();
};
