#include "cpu.hpp"

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <format>
#include <fstream>
#include <iostream>

uint8_t get_byte (std::ifstream &fptr);

int
main (void)
{
  const std::string FILENAME = "helloworld.rom";
  std::ifstream file (FILENAME, std::ios::binary);
  if (!file)
    {
      std::cerr << "Error opening file: " << FILENAME << std::endl;
      return -1;
    }

  uint8_t byte_value = get_byte (file);

  CPU cpu;

  assert (uint8_t (byte_value / 16) == 6);

  CPU::data_register reg = CPU::get_data_register (byte_value % 16);
  std::cout << std::format ("Before: {:02X}\n", cpu.registers[reg]);

  byte_value = get_byte (file);
  cpu.registers[reg] = byte_value;

  std::cout << std::format ("After: {:02X}\n", cpu.registers[reg]);

  return 0;
}

uint8_t
get_byte (std::ifstream &fptr)
{
  char byte;
  fptr.read (&byte, sizeof (byte));
  assert (fptr);

  return static_cast<uint8_t> (byte);
}
