#include "cpu.hpp"

#include <format>
#include <iostream>
#include <stdexcept>
#include <string>

CPU::data_register
CPU::get_data_register (uint8_t x)
{
  switch (x)
    {
    case 0x0:
      return CPU::V0;
    case 0x1:
      return CPU::V1;
    case 0x2:
      return CPU::V2;
    case 0x3:
      return CPU::V3;
    case 0x4:
      return CPU::V4;
    case 0x5:
      return CPU::V5;
    case 0x6:
      return CPU::V6;
    case 0x7:
      return CPU::V7;
    case 0x8:
      return CPU::V8;
    case 0x9:
      return CPU::V9;
    case 0xA:
      return CPU::VA;
    case 0xB:
      return CPU::VB;
    case 0xC:
      return CPU::VC;
    case 0xD:
      return CPU::VD;
    case 0xE:
      return CPU::VE;
    case 0xF:
      return CPU::VF;
    default:
      throw std::runtime_error ("Invalid register, " + std::to_string (x));
    }
}

void
CPU::set_memory_address_register (uint16_t addr)
{
  memory_address_register = addr & 0xFFF;
}

void
CPU::set_data_register_to_nn (uint16_t input)
{
  CPU::data_register reg = CPU::get_data_register ((input >> 8) & 0xF);
  std::cout << std::format ("Register {} before: {:02X}\n",
                            static_cast<int> (reg), registers[reg]);

  registers[reg] = input & 0xFF;
  std::cout << std::format ("Register {} after: {:02X}\n",
                            static_cast<int> (reg), registers[reg]);
}
