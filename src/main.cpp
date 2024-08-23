#include "cpu.hpp"

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <format>
#include <fstream>
#include <iostream>

int
main (void)
{
  // const std::string FILENAME = "helloworld.rom";
  const std::string FILENAME = "cpy.rom";
  std::ifstream file (FILENAME, std::ios::binary);
  if (!file)
    {
      std::cerr << "Error opening file: " << FILENAME << std::endl;
      return -1;
    }

  char byte;
  file.read (&byte, sizeof (byte));
  if (!file)
    {
      std::cerr << "Error reading byte from file.\n";
      return -1;
    }

  uint8_t byte_value = static_cast<uint8_t> (byte);
  if (file)
    {
      std::cout << std::format ("Byte: {:02X}\n", byte_value);
    }
  else
    {
      std::cout << "Error reading from file.\n";
      return -1;
    }

  CPU cpu;
  assert (uint8_t (byte_value / 16) == 6);
  // printf ("%d\n", CPU::get_data_register (byte_value % 16));
  CPU::data_register reg = CPU::get_data_register (byte_value % 16);
  std::cout << std::format ("{}\n", static_cast<int> (reg));
  printf ("%02X\n", cpu.registers[CPU::get_data_register (byte_value % 16)]);

  return 0;
}
