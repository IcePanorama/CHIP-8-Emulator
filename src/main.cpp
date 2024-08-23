#include "cpu.hpp"

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <format>
#include <fstream>
#include <iostream>

uint8_t get_byte (std::ifstream &fptr);
uint16_t get_uint16 (std::ifstream &fptr);

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

  uint16_t opcode = get_uint16 (file);

  CPU cpu;

  std::cout << std::format ("Opcode: {:04X}\n", opcode);
  assert (uint8_t (opcode >> 12) == 6);

  CPU::data_register reg = CPU::get_data_register ((opcode >> 8) % 0xF);
  std::cout << std::format ("Before: {:02X}\n", cpu.registers[reg]);

  cpu.registers[reg] = opcode & 0xFF;
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

uint16_t
get_uint16 (std::ifstream &fptr)
{
  uint8_t low_byte = get_byte (fptr);
  uint8_t high_byte = get_byte (fptr);
  return (static_cast<uint16_t> (low_byte) << 8) | high_byte;
}
