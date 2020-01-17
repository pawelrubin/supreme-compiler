#pragma once

#include "typedefs.hpp"

#include <string>

enum class Atomic {
  GET,
  PUT,
  LOAD,
  STORE,
  LOADI,
  STOREI,
  ADD,
  SUB,
  SHIFT,
  INC,
  DEC,
  JUMP,
  JPOS,
  JZERO,
  JNEQ,
  HALT
};

class Instruction {
protected:
  integer pc;
  std::string code;

public:
  void set_pc(integer);
  integer get_pc();
  virtual std::string assembly() = 0;
};

class SimpleInstruction : public Instruction {
public:
  std::string assembly() override;
};

class MemoryInstruction : public Instruction {
protected:
  integer address;
public:
  std::string assembly() override;
  explicit MemoryInstruction(integer a) : address(a) {}
};

class JumpInstruction : public Instruction {
private:
  Instruction* destination;
public:
  JumpInstruction(Instruction* d) : destination(d) {}
  std::string assembly() override;
};

class Get : public SimpleInstruction {
public:
  Get() : SimpleInstruction() { 
    this->code = "GET"; 
  }
};

class Put : public SimpleInstruction {
public:
  Put() : SimpleInstruction() { 
    this->code = "PUT"; 
  }
};

class Inc : public SimpleInstruction {
public:
  Inc() : SimpleInstruction() { 
    this->code = "INC"; 
  }
};

class Dec : public SimpleInstruction {
public:
  Dec() : SimpleInstruction() { 
    this->code = "DEC"; 
  }
};

class Halt : public SimpleInstruction {
public:
  Halt() : SimpleInstruction() { 
    this->code = "HALT"; 
  }
};

class Load : public MemoryInstruction {
public:
  Load(integer a) : MemoryInstruction(a) { 
    this->code = "LOAD"; 
  }
};

class Store : public MemoryInstruction {
public:
  Store(integer a) : MemoryInstruction(a) { 
    this->code = "STORE"; 
  }
};

class Loadi : public MemoryInstruction {
public:
  Loadi(integer a) : MemoryInstruction(a) { 
    this->code = "LOADI"; 
  }
};

class Storei : public MemoryInstruction {
public:
  Storei(integer a) : MemoryInstruction(a) { 
    this->code = "STOREI"; 
  }
};

class Add : public MemoryInstruction {
public:
  Add(integer a) : MemoryInstruction(a) { 
    this->code = "ADD"; 
  }
};

class Sub : public MemoryInstruction {
public:
  Sub(integer a) : MemoryInstruction(a) { 
    this->code = "SUB"; 
  }
};

class Shift : public MemoryInstruction {
public:
  Shift(integer a) : MemoryInstruction(a) { 
    this->code = "SHIFT"; 
  }
};

class Jump : public JumpInstruction {
public:
  Jump(Instruction* i) : JumpInstruction(i) {
    this->code = "JUMP";
  }
};

class Jpos : public JumpInstruction {
public:
  Jpos(Instruction* i) : JumpInstruction(i) {
    this->code = "JPOS";
  }
};

class Jzero : public JumpInstruction {
public:
  Jzero(Instruction* i) : JumpInstruction(i) {
    this->code = "JZERO";
  }
};

class Jneg : public JumpInstruction {
public:
  Jneg(Instruction* i) : JumpInstruction(i) {
    this->code = "JNEG";
  }
};
