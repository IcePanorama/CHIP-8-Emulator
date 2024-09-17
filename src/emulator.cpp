#include "emulator.hpp"
#include "cpu.hpp"
#include "utils.hpp"

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
      cpu.handle_jump_opcode (opcode);
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

void
Emulator::load_program (std::ifstream &p)
{
  for (uint16_t i = CPU::PROGRAM_MEM_START; i < CPU::CALL_STACK_MEM_START; i++)
    {
      uint8_t opcode = get_uint8 (p);
      if (p.eof ())
        return;

      cpu.memory[i] = opcode;
    }

  std::runtime_error ("Out of memory error.");
}

void
Emulator::run (void)
{
  // Resetting these to their default values
  cpu.program_counter = CPU::PROGRAM_MEM_START;
  cpu.stack_pointer = CPU::CALL_STACK_MEM_START;

  for (; cpu.program_counter < CPU::CALL_STACK_MEM_START;
       cpu.program_counter += 2)
    {
      uint16_t opcode = cpu.memory[cpu.program_counter] << 8;
      opcode |= cpu.memory[cpu.program_counter + 1];
      process_opcode (opcode);
    }
}

#define BYTES_TO_READ 32
void
Emulator::inspect_memory (uint16_t start_loc)
{
  for (uint16_t i = start_loc; i < start_loc + BYTES_TO_READ; i++)
    {
      std::cout << std::format (" {:02X}", cpu.memory[i]);
      uint16_t tmp = i - start_loc;
      if (tmp != 0)
        {
          if ((tmp % 4) == 3)
            std::cout << "\t";
          if ((tmp % 16) == 15)
            std::cout << std::endl;
        }
    }
}
