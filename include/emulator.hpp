#ifndef _EMULATOR_HPP_
#define _EMULATOR_HPP_

#include "cpu.hpp"

#include <cstdint>

class Emulator
{
  CPU cpu;

public:
  Emulator (void) = default;
  void load_program (std::ifstream &p);
  void process_opcode (uint16_t opcode);
  void run (void);

  /**
   *  Prints `BYTES_TO_READ` many bytes from `memory`, starting from
   *  `start_loc`.
   */
  void inspect_memory (uint16_t start_loc);
};

#endif
