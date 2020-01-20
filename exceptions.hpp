#pragma once

#include <exception>
#include <iostream>

class NOPAsLastInstructionException : public std::exception {
  virtual const char* what() const throw() {
    return "Bad use of NOP instruction. NOP can't be the last instruction.";
  }
};
