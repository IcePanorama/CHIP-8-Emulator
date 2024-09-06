#ifndef _EMULATOR_HPP_
#define _EMULATOR_HPP_

#include "cpu.hpp"

#include <array>
#include <cstdint>

#define NUM_FONT_SPRITES 80
// in bytes
#define FONT_SPRITE_SIZE 5

class Emulator
{
  CPU cpu;

  /** Copies font sprite data into `memory` at location `FONT_MEM_START`. */
  void init_font_sprites (void);

  /**
   *  Given a uint8_t character, `ch`, returns the location of its sprite in
   *  memory if it is a valid character (`0x0`-`0xF`) else it throws a runtime
   *  exception.
   */
  uint16_t get_font_sprite_mem_loc (uint8_t ch);

  /** Set cpu data register X to NN. Handles 6XNN opcode. */
  void handle_set_vx_to_nn_opcode (uint16_t input);

  /** Set the 12-bit address register (I) to NNN. Handles ANNN opcode. */
  void handle_set_i_to_nnn_opcode (uint16_t input);

  /**
   *  Awaits a key press and then stores that key into cpu data register X.
   *  Handles FX0A opcode.
   */
  void handle_get_key_opcode (uint16_t input);

  /** Clears the screen. Handles 00E0 opcode. */
  void handle_clear_display_opcode (void);

  /**
   *  Sets the sound timer to the value stored in cpu data register X. Handles
   *  FX18 opcode.
   */
  void handle_set_sound_timer_opcode (uint16_t input);

  /**
   *  Sets the 12-bit address register (I) to the location in memory of the
   *  font sprite for the character stored in cpu data register X. Handles FX29
   *  opcode.
   */
  void handle_set_i_to_sprite_loc_opcode (uint16_t input);

  /**
   *  Draws the sprite stored in the location held by the address register to
   *  a location on screen using the values stored in cpu data register X and Y
   *  as its X and Y coordinates. Sets cpu data register VF to 1 if any pixel
   *  is flipped, otherwise it sets it to 0. Handles DXYN opcode.
   */
  void handle_draw_sprite_opcode (uint16_t input);

  /** Jumps to address NNN. Handles 1NNN opcode. */
  void handle_goto_opcode (uint16_t input);

public:
  Emulator (void) { init_font_sprites (); }

  void process_opcode (uint16_t opcode);
};

#endif
