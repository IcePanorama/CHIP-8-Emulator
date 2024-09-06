#include "emulator.hpp"

#include <array>
#include <cstdint>
#include <cstring>
#include <format>
#include <iostream>
#include <ostream>
#include <stdexcept>

/* See: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#2.4 */
void
Emulator::init_font_sprites (void)
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
    cpu.memory[i] = fontset[i];
}

uint16_t
Emulator::get_font_sprite_mem_loc (uint8_t ch)
{
  std::cout << "get_font_sprite_mem_loc";

  if (ch > 0xF)
    throw std::runtime_error (std::format ("Invalid character, %d.\n", ch));

  return ch * FONT_SPRITE_SIZE;
}

void
Emulator::handle_set_vx_to_nn_opcode (uint16_t input)
{
  std::cout << "handle_set_vx_to_nn_opcode";

  CPU::DataRegister_t reg = CPU::get_data_register ((input >> 8) & 0xF);
  cpu.registers[reg] = input & 0xFF;
}

void
Emulator::handle_set_i_to_nnn_opcode (uint16_t input)
{
  std::cout << "handle_set_i_to_nnn_opcode";
  cpu.set_address_register (input & 0xFFF);
}

void
Emulator::handle_get_key_opcode (uint16_t input)
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
    throw std::runtime_error (std::format ("Invalid key, %c.\n", c));

  CPU::DataRegister_t reg = CPU::get_data_register ((input >> 8) & 0xF);
  cpu.registers[reg] = keycode;
}

void
Emulator::handle_clear_display_opcode (void)
{
  std::cout << "handle_get_key_opcode";
  std::cout << " - Clearing the screen...";
}

void
Emulator::handle_set_sound_timer_opcode (uint16_t input)
{
  std::cout << "handle_set_sound_timer_opcode";

  CPU::DataRegister_t reg = CPU::get_data_register ((input >> 8) & 0xF);
  cpu.sound_timer = cpu.registers[reg];
}

void
Emulator::handle_set_i_to_sprite_loc_opcode (uint16_t input)
{
  std::cout << "handle_set_i_to_sprite_loc_opcode";
  CPU::DataRegister_t reg = CPU::get_data_register ((input >> 8) & 0xF);

  uint16_t sprite_loc = 0;
  try
    {
      sprite_loc = get_font_sprite_mem_loc (cpu.registers[reg]);
    }
  catch (std::runtime_error const &)
    {
      throw std::runtime_error (
          std::format ("Could not locate font sprite for character, %02X.\n",
                       cpu.registers[reg]));
    }

  cpu.set_address_register (sprite_loc);
}

void
Emulator::handle_draw_sprite_opcode (uint16_t input)
{
  std::cout << "handle_draw_sprite_opcode";

  CPU::DataRegister_t x_pos_reg = CPU::get_data_register ((input >> 8) & 0xF);
  CPU::DataRegister_t y_pos_reg = CPU::get_data_register ((input >> 4) & 0xF);

  uint8_t x_pos = cpu.registers[x_pos_reg];
  uint8_t y_pos = cpu.registers[y_pos_reg];
  std::cout << std::format (" - draw coords: ({:02X},{:02X})", x_pos, y_pos);
}

void
Emulator::handle_goto_opcode (uint16_t input)
{
  std::cout << "handle_goto_opcode";
  uint16_t new_addr = input & 0xFFF;

  std::cout << std::format (" - Jumping to address {:03X}", new_addr);
}

void
Emulator::process_opcode (uint16_t opcode)
{
  std::cout << cpu << std::endl;
  std::cout << std::format ("Opcode: {:04X} - ", opcode);

  // Primarily worried about the most significant nibble.
  switch (uint8_t (opcode >> 12))
    {
    case 0x0:
      if (opcode == 0x00E0)
        {
          handle_clear_display_opcode ();
          break;
        }

      [[fallthrough]];
    case 0x1:
      handle_goto_opcode (opcode);
      break;
    case 0x6:
      handle_set_vx_to_nn_opcode (opcode);
      break;
    case 0xA:
      handle_set_i_to_nnn_opcode (opcode);
      break;
    case 0xD:
      handle_draw_sprite_opcode (opcode);
      break;
    case 0xF:
      if ((opcode & 0xFF) == 0x0A) // FX0A
        {
          handle_get_key_opcode (opcode);
          break;
        }
      else if ((opcode & 0xFF) == 0x18) // FX18
        {
          handle_set_sound_timer_opcode (opcode);
          break;
        }
      else if ((opcode & 0xFF) == 0x29) // FX29
        {
          handle_set_i_to_sprite_loc_opcode (opcode);
          break;
        }
      [[fallthrough]];
    default:
      throw std::runtime_error (
          std::format ("Unknown opcode: {:04X}\n", opcode));
    }

  std::cout << std::endl << std::endl;
}
