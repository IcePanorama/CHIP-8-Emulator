#include "emulator.hpp"

#include <cstdint>
#include <cstring>
#include <format>
#include <iostream>
#include <ostream>
#include <stdexcept>

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
          cpu.handle_clear_display_opcode ();
          break;
        }

      [[fallthrough]];
    case 0x1:
      cpu.handle_goto_opcode (opcode);
      break;
    case 0x6:
      cpu.handle_set_vx_to_nn_opcode (opcode);
      break;
    case 0xA:
      cpu.handle_set_i_to_nnn_opcode (opcode);
      break;
    case 0xD:
      cpu.handle_draw_sprite_opcode (opcode);
      break;
    case 0xF:
      if ((opcode & 0xFF) == 0x0A) // FX0A
        {
          cpu.handle_get_key_opcode (opcode);
          break;
        }
      else if ((opcode & 0xFF) == 0x18) // FX18
        {
          cpu.handle_set_sound_timer_opcode (opcode);
          break;
        }
      else if ((opcode & 0xFF) == 0x29) // FX29
        {
          cpu.handle_set_i_to_sprite_loc_opcode (opcode);
          break;
        }
      [[fallthrough]];
    default:
      throw std::runtime_error (
          std::format ("Unknown opcode: {:04X}\n", opcode));
    }

  std::cout << std::endl << std::endl;
}
