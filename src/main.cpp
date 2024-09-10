#include "cpu.hpp"
#include "emulator.hpp"
#include "utils.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

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
  std::cout << "Loading program into memory...\n";
  try
    {
      emu.load_program (file);
    }
  catch (std::runtime_error &)
    {
      exit (EXIT_FAILURE);
    }

  emu.cpu.inspect_memory (CPU::PROGRAM_MEM_START);

  std::cout << "Starting emulator...\n";
  /*
    while (true)
      {
        uint16_t opcode = get_uint16 (file);
        if (file.eof ())
          break;

        emu.process_opcode (opcode);
      }
  */

  std::cout << "Goodbye.\n";

  return 0;
}
