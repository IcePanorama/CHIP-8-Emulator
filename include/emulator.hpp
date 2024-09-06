#ifndef _EMULATOR_HPP_
#define _EMULATOR_HPP_

#include "cpu.hpp"
// #include "sprite.hpp"

#include <array>
#include <cstdint>

#define NUM_FONT_SPRITES 80
// in bytes
#define FONT_SPRITE_SIZE 5

class Emulator
{
  CPU cpu;
  std::array<uint8_t, 4096> memory = { 0 };

  // std::array<Sprite, 16> global_sprites;
  // bool sprites_initd = false; // may be unnecessary

  /** Specific locations in `memory` that are worth having direct access to. */
  typedef enum MemoryLocation_e
  {
    FONT_MEM_START = 0x0,
    PROGRAM_MEM_START = 0x200,
    CALL_STACK_MEM_START = 0xEA0, // Also used for internal use/other vars
    DISPLAY_MEM_START = 0xFFF,
  } MemoryLocation_t;

  /** Copies font sprite data into `memory` at location `FONT_MEM_START`. */
  void init_font_sprites (void);

  /**
   *  Given a uint8_t character, `ch`, returns the location of its sprite in
   *  memory if it is a valid character (`0x0`-`0xF`) else it throws a runtime
   *  exception.
   */
  uint16_t get_font_sprite_mem_loc (uint8_t ch);

  // TODO: rename all of these.
  /** Handle `6XNN` opcode. */
  void handle_set_data_register_to_nn_opcode (uint16_t input);
  /** Handle `ANNN` opcode. */
  void handle_set_address_register_opcode (uint16_t input);
  /** Handle `FX0A` opcode. */
  void handle_get_key_opcode (uint16_t input);
  /** Handle `00E0` opcode. */
  void handle_clear_display_opcode (void);
  /** Handle `FX18` opcode. */
  void handle_sound_timer_opcode (uint16_t input);
  /** Handle `FX29` opcode. */
  void handle_set_addr_reg_to_loc_of_sprite_opcode (uint16_t input);
  /** Handle `DXYN` opcode. */
  void handle_draw_opcode (uint16_t input);
  /** Handle `1NNN` opcode. */
  void handle_goto_opcode (uint16_t input);

public:
  Emulator (void) { init_font_sprites (); }

  void process_opcode (uint16_t opcode);
};

#endif
