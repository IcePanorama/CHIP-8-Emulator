#include "cpu.hpp"

#include <cstdio>
#include <format>
#include <iostream>
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

/* See: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#2.4 */
void
CPU::init_font_sprites (void)
{
  /* clang-format off */
  constexpr std::array<uint8_t, NUM_FONT_SPRITES> fontset = {
    // 0
    0xF0, 0x90, 0x90, 0x90, 0xF0,
    // 1
    0x20, 0x60, 0x20, 0x20, 0x70,
    // 2
    0xF0, 0x10, 0xF0, 0x80, 0xF0,
    // 3
    0xF0, 0x10, 0xF0, 0x10, 0xF0,
    // 4
    0x90, 0x90, 0xF0, 0x10, 0x10,
    // 5
    0xF0, 0x80, 0xF0, 0x10, 0xF0,
    // 6
    0xF0, 0x80, 0xF0, 0x90, 0xF0,
    // 7
    0xF0, 0x10, 0x20, 0x40, 0x40,
    // 8
    0xF0, 0x90, 0xF0, 0x90, 0xF0,
    // 9
    0xF0, 0x90, 0xF0, 0x10, 0xF0,
    // A
    0xF0, 0x90, 0xF0, 0x90, 0x90,
    // B
    0xE0, 0x90, 0xE0, 0x90, 0xE0,
    // C
    0xF0, 0x80, 0x80, 0x80, 0xF0,
    // D
    0xE0, 0x90, 0x90, 0x90, 0xE0,
    // E
    0xF0, 0x80, 0xF0, 0x80, 0xF0,
    // F
    0xF0, 0x80, 0xF0, 0x80, 0x80,
  };
  /* clang-format on */

  // Trying to avoid using `memcpy` as I know that's the Devil in C++
  for (uint8_t i = 0; i < NUM_FONT_SPRITES; i++)
    memory[i] = fontset[i];
}

uint16_t
CPU::get_font_sprite_mem_loc (uint8_t ch)
{
  std::cout << "get_font_sprite_mem_loc";

  if (ch > 0xF)
    throw std::runtime_error (std::format ("Invalid character, %d.\n", ch));

  return ch * FONT_SPRITE_SIZE;
}

void
CPU::handle_set_vx_to_nn_opcode (uint16_t input)
{
  std::cout << "handle_set_vx_to_nn_opcode";

  CPU::DataRegister_t reg = CPU::get_data_register ((input >> 8) & 0xF);
  registers[reg] = input & 0xFF;
}

void
CPU::handle_set_i_to_nnn_opcode (uint16_t input)
{
  std::cout << "handle_set_i_to_nnn_opcode";
  set_address_register (input & 0xFFF);
}

void
CPU::handle_get_key_opcode (uint16_t input)
{
  std::cout << "handle_get_key_opcode";

  std::cout << std::endl; // for better readability, may not need this later

  // User must currently press enter for keycode to be registered
  // This'll be changed when I switch to using raylib or whatever.
  char c = getchar ();
  uint8_t keycode = 0;

  if (c >= 'a' && c <= 'f')
    keycode = c - 'a' + 10;
  else if (c >= 'A' && c <= 'F')
    keycode = c - 'A' + 10;
  else if (c >= '0' && c <= '9')
    keycode = c - '0';
  else
    throw std::runtime_error (std::format ("Invalid key, {:c}.\n", c));

  CPU::DataRegister_t reg = CPU::get_data_register ((input >> 8) & 0xF);
  registers[reg] = keycode;
}

void
CPU::handle_clear_display_opcode (void)
{
  std::cout << "handle_clear_display_opcode";
  for (uint16_t i = CPU::DISPLAY_MEM_START; i < CPU::END_OF_MEMORY; i++)
    {
      memory[i] = 0;
    }
}

void
CPU::handle_set_sound_timer_opcode (uint16_t input)
{
  std::cout << "handle_set_sound_timer_opcode";

  CPU::DataRegister_t reg = CPU::get_data_register ((input >> 8) & 0xF);
  sound_timer = registers[reg];
}

void
CPU::handle_set_i_to_sprite_loc_opcode (uint16_t input)
{
  std::cout << "handle_set_i_to_sprite_loc_opcode";
  CPU::DataRegister_t reg = CPU::get_data_register ((input >> 8) & 0xF);

  uint16_t sprite_loc = 0;
  try
    {
      sprite_loc = get_font_sprite_mem_loc (registers[reg]);
    }
  catch (std::runtime_error const &)
    {
      throw std::runtime_error (
          std::format ("Could not locate font sprite for character, %02X.\n",
                       registers[reg]));
    }

  set_address_register (sprite_loc);
}

void
CPU::handle_draw_sprite_opcode (uint16_t input)
{
  std::cout << "handle_draw_sprite_opcode";

  CPU::DataRegister_t x_pos_reg = CPU::get_data_register ((input >> 8) & 0xF);
  CPU::DataRegister_t y_pos_reg = CPU::get_data_register ((input >> 4) & 0xF);

  uint8_t x_pos = registers[x_pos_reg];
  uint8_t y_pos = registers[y_pos_reg];
  std::cout << std::format (" - draw coords: ({:02X},{:02X})", x_pos, y_pos);
}

void
CPU::handle_goto_opcode (uint16_t input)
{
  std::cout << "handle_goto_opcode";
  program_counter = input & 0xFFF;

  std::cout << std::format (" - Jumping to address {:03X}", program_counter);
}
