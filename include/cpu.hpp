#ifndef _CPU_HPP_
#define _CPU_HPP_

#include <array>
#include <cstdint>
#include <ostream>

#define NUM_FONT_SPRITES 80
// in bytes
#define FONT_SPRITE_SIZE 5

class CPU
{
  /**
   *  12-bit address register (I). Using get/set methods to enforce the 12-bit
   *  constraint.
   */
  uint16_t address_register = 0;

  /** Copies font sprite data into `memory` at location `FONT_MEM_START`. */
  void init_font_sprites (void);

public:
  std::array<uint8_t, 16> registers = { 0 }; // 8-bit data registers, V0 to VF.
  std::array<uint8_t, 4096> memory = { 0 };
  uint8_t delay_timer = 0;
  uint8_t sound_timer = 0;

  /** Specific locations in `memory` that are worth having direct access to. */
  typedef enum MemoryLocation_e
  {
    FONT_MEM_START = 0x0,
    PROGRAM_MEM_START = 0x200,
    CALL_STACK_MEM_START = 0xEA0, // Also used for internal use/other vars
    DISPLAY_MEM_START = 0xF00,
    END_OF_MEMORY = 0xFFF,
  } MemoryLocation_t;

  uint16_t program_counter = CALL_STACK_MEM_START;

  /* clang-format off */
  typedef enum DataRegister_e
  {
    V0, V1, V2, V3, V4, V5, V6, V7, V8, V9, VA, VB, VC, VD, VE,
    VF, // doubles as carry/no-borrow/other flag
  } DataRegister_t;
  /* clang-format on */

  static DataRegister_t get_data_register (uint8_t x);

  void set_address_register (uint16_t addr);
  uint16_t get_address_register (void) const;

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

  CPU (void) { init_font_sprites (); };
};

std::ostream &operator<< (std::ostream &os, const CPU &c);

#endif
