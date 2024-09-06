#ifndef _EMULATOR_HPP_
#define _EMULATOR_HPP_

#include "cpu.hpp"
#include "sprite.hpp"

#include <array>
#include <cstdint>

class Emulator
{
  CPU cpu;
  std::array<uint8_t, 4096> memory;
  std::array<Sprite, 16> global_sprites;
  bool sprites_initd = false; // may be unnecessary

  /** Specific locations in `memory` that are worth having direct access to. */
  typedef enum MemoryLocation_e
  {
    MEMORY_START = 0x0,
    PROGRAM_MEM_START = 0x200,
    // Supposedly used for internal use/other vars also
    CALL_STACK_MEM_START = 0xEA0,
    DISPLAY_MEM_START = 0xFFF,
  } MemoryLocation_t;

  void init_global_sprites (void);
  void handle_set_data_register_to_nn_opcode (uint16_t input);
  void handle_set_address_register_opcode (uint16_t input);
  void handle_get_key_opcode (uint16_t input);
  void handle_clear_display_opcode (void);
  void handle_sound_timer_opcode (uint16_t input);
  void handle_set_addr_reg_to_loc_of_sprite_opcode (uint16_t input);
  void handle_draw_opcode (uint16_t input);
  void handle_goto_opcode (uint16_t input);

public:
  Emulator (void) { init_global_sprites (); }

  void process_opcode (uint16_t opcode);
};

#endif
