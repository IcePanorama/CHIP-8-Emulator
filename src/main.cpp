#include "emulator.hpp"
#include "utils.hpp"

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iostream>

int
main (void)
{
  const std::string FILENAME = "helloworld.rom";
  std::cout << "Loading file, " << FILENAME << "...\n";
  std::ifstream file (FILENAME, std::ios::binary);
  if (!file)
    {
      std::cerr << "Error opening file: " << FILENAME << std::endl;
      return -1;
    }

  Emulator emu;
  std::cout << "Starting emulator...\n";
  while (true)
    {
      uint16_t opcode = get_uint16 (file);
      if (file.eof ())
        break;

      emu.process_opcode (opcode);
    }

  std::cout << "Goodbye.\n";

  return 0;
}
