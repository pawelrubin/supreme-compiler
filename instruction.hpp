#pragma once


#include <string>
#include <vector>

#include "typedefs.hpp"
#include "data.hpp"

extern Data* data;

class Instruction;
class JumpInstruction;
class InstructionVector;
class Assembler;

typedef std::vector<Instruction*> InstructionList;

class InstructionVector {
protected:
  std::vector<Instruction*> instructions;

public:
  InstructionVector& push(Instruction*);
  InstructionVector& append(InstructionVector);
  std::vector<Instruction*>* get_vector();
  Instruction* begin();
};

class InstructionVectorWithJump : public InstructionVector {
  private:
    JumpInstruction* jump;
  public:
    JumpInstruction* get_jump();
    void set_jump(JumpInstruction*);
};

class CodeGenerator {
private:
  InstructionVector instructions;

public:
  CodeGenerator(InstructionVector v) : instructions(v) {} 
  void peephole();
  codeList generateCode();
};

class Instruction {
protected:
  integer pc;
  std::string code;
  bool safe_to_delete;

public:
  Instruction() : safe_to_delete(true) {}
  void set_pc(integer);
  void set_unsafe();
  bool is_safe_to_delete();
  integer get_pc();
  virtual std::string assembly() = 0;
  std::string get_code();
};

class SimpleInstruction : public Instruction {
public:
  using Instruction::Instruction;
  std::string assembly() override;
};

class MemoryInstruction : public Instruction {
protected:
  integer address;
public:
  explicit MemoryInstruction(integer a) : Instruction(), address(a) {}
  explicit MemoryInstruction(Register);
  integer get_address();
  std::string assembly() override;
};

class JumpInstruction : public Instruction {
private:
  Instruction* jump_destination;
public:
  using Instruction::Instruction;
  JumpInstruction(Instruction* i) : jump_destination(i) {} 
  std::string assembly() override;
  void set_jump_destination(Instruction*);
};

class NOP : public Instruction {
public:
  std::vector<JumpInstruction*> jumpers;
  NOP() = default;
  NOP(JumpInstruction*);
  void add(JumpInstruction*);
  std::string assembly() override { return ""; }
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
  Load(Register r) : MemoryInstruction(r) { 
    this->code = "LOAD"; 
  }
  Load(integer a) : MemoryInstruction(a) { 
    this->code = "LOAD"; 
  }
};

class Store : public MemoryInstruction {
public:
  Store(Register r) : MemoryInstruction(r) {
    this->code = "STORE";  
  }
  Store(integer a) : MemoryInstruction(a) { 
    this->code = "STORE"; 
  }
};

class Loadi : public MemoryInstruction {
public:
  Loadi(Register r) : MemoryInstruction(r) { 
    this->code = "LOADI"; 
  }
  Loadi(integer a) : MemoryInstruction(a) { 
    this->code = "LOADI"; 
  }
};

class Storei : public MemoryInstruction {
public:
  Storei(Register r) : MemoryInstruction(r) { 
    this->code = "STOREI"; 
  }
  Storei(integer a) : MemoryInstruction(a) { 
    this->code = "STOREI"; 
  }
};

class Add : public MemoryInstruction {
public:
  Add(Register r) : MemoryInstruction(r) { 
    this->code = "ADD"; 
  }
  Add(integer a) : MemoryInstruction(a) { 
    this->code = "ADD"; 
  }
};

class Sub : public MemoryInstruction {
public:
  Sub(Register r) : MemoryInstruction(r) { 
    this->code = "SUB"; 
  }
  Sub(integer a) : MemoryInstruction(a) { 
    this->code = "SUB"; 
  }
};

class Shift : public MemoryInstruction {
public:
  Shift(Register r) : MemoryInstruction(r) { 
    this->code = "SHIFT"; 
  }
  Shift(integer a) : MemoryInstruction(a) { 
    this->code = "SHIFT"; 
  }
};

class Jump : public JumpInstruction {
public:
  Jump(Instruction* i) : JumpInstruction(i) {
    this->code = "JUMP";
  }
  Jump() : JumpInstruction() {
    this->code = "JUMP";
  }
};

class Jpos : public JumpInstruction {
public:
  Jpos(Instruction* i) : JumpInstruction(i) {
    this->code = "JPOS";
  }
  Jpos() : JumpInstruction() {
    this->code = "JPOS";
  }
};

class Jzero : public JumpInstruction {
public:
  Jzero(Instruction* i) : JumpInstruction(i) {
    this->code = "JZERO";
  }
  Jzero() : JumpInstruction() {
    this->code = "JZERO";
  }
};

class Jneg : public JumpInstruction {
public:
  Jneg(Instruction* i) : JumpInstruction(i) {
    this->code = "JNEG";
  }
  Jneg() : JumpInstruction() {
    this->code = "JNEG";
  }
};
