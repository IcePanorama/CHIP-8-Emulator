#include "cpu.hpp"

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <format>
#include <fstream>
#include <iostream>

uint8_t get_byte (std::ifstream &fptr);
uint16_t get_uint16 (std::ifstream &fptr);
void set_data_register_to_nn (CPU &cpu, uint16_t input);
void handle_set_memory_address_register_opcode (CPU &cpu, uint16_t input);

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

  while (!file.eof ())
    {
      uint16_t opcode = get_uint16 (file);
      if (file.eof ())
        break;

      std::cout << std::format ("Opcode: {:04X}\n", opcode);

      switch (uint8_t (opcode >> 12))
        {
        case 0x6:
          set_data_register_to_nn (cpu, opcode);
          break;
        case 0xA:
          handle_set_memory_address_register_opcode (cpu, opcode);
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

void
set_data_register_to_nn (CPU &cpu, uint16_t input)
{
  CPU::data_register reg = CPU::get_data_register ((input >> 8) & 0xF);
  std::cout << std::format ("Register {} before: {:02X}\n",
                            static_cast<int> (reg), cpu.registers[reg]);

  cpu.registers[reg] = input & 0xFF;
  std::cout << std::format ("Register {} after: {:02X}\n",
                            static_cast<int> (reg), cpu.registers[reg]);
}

void
handle_set_memory_address_register_opcode (CPU &cpu, uint16_t input)
{
  std::cout << std::format ("Before: {:03X}\n",
                            cpu.get_memory_address_register ());
  cpu.set_memory_address_register (input & 0xFFF);
  std::cout << std::format ("After: {:03X}\n",
                            cpu.get_memory_address_register ());
}
