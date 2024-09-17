#include "cpu.hpp"
#include "emulator.hpp"

#include <cstdlib>
#include <fstream>
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

  emu.inspect_memory (CPU::PROGRAM_MEM_START);

  std::cout << "Starting emulator...\n";
  emu.run ();

  std::cout << "Goodbye.\n";

  return 0;
}
