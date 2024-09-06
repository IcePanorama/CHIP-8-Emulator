#include "cpu.hpp"

#include <format>
#include <stdexcept>
#include <string>

CPU::DataRegister_t
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
CPU::set_address_register (uint16_t addr)
{
  address_register = addr & 0xFFF;
}

uint16_t
CPU::get_address_register (void) const
{
  return address_register;
}

std::ostream &
operator<< (std::ostream &os, const CPU &c)
{
  os << "{";

  // Address register
  os << std::format ("i={:03X}", c.get_address_register ());

  os << ", ";

  /* Data Registers */
  os << "{";
  os << std::format (
      "V0: {:02X}, V1: {:02X}, V2: {:02X}, V3: {:02X}, V4: "
      "{:02X}, V5: {:02X}, V6: {:02X}, V7: {:02X}, V8: {:02X} "
      " V9: {:02X} VA: {:02X}, VB: {:02X}, VC: {:02X}, VD: "
      "{:02X}, VE: {:02X} VF: {:02X}",
      c.registers[CPU::V0], c.registers[CPU::V1], c.registers[CPU::V2],
      c.registers[CPU::V3], c.registers[CPU::V4], c.registers[CPU::V5],
      c.registers[CPU::V6], c.registers[CPU::V7], c.registers[CPU::V8],
      c.registers[CPU::V9], c.registers[CPU::VA], c.registers[CPU::VB],
      c.registers[CPU::VC], c.registers[CPU::VD], c.registers[CPU::VE],
      c.registers[CPU::VF]);
  os << "}";

  os << ", ";

  os << std::format ("sound timer: {:02X}", c.sound_timer);

  os << "}";

  return os;
}
