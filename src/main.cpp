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
  CPU cpu;

  while (true)
    {
      uint16_t opcode = get_uint16 (file);
      if (file.eof ())
        break;

      std::cout << std::format ("Opcode: {:04X}\n", opcode);

      switch (uint8_t (opcode >> 12))
        {
        case 0x6:
          cpu.set_data_register_to_nn (opcode);
          break;
        case 0xA:
          break;
        default:
          std::cout << std::format ("Unknown opcode: {:04X}\n", opcode);
        }
    }

  return 0;
}

uint8_t
get_byte (std::ifstream &fptr)
{
  char byte;
  fptr.read (&byte, sizeof (byte));

  return static_cast<uint8_t> (byte);
}

uint16_t
get_uint16 (std::ifstream &fptr)
{
  uint8_t low_byte = get_byte (fptr);
  uint8_t high_byte = get_byte (fptr);
  return (static_cast<uint16_t> (low_byte) << 8) | high_byte;
}
