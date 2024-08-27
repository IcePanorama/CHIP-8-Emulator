#ifndef _EMULATOR_HPP_
#define _EMULATOR_HPP_

#include "cpu.hpp"
#include "sprite.hpp"

#include <stdint.h>

class Emulator
{
  // may be unnecessary
  bool sprites_initd = false;
  CPU cpu;
  std::array<Sprite, 16> global_sprites;

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
