#ifndef _CPU_HPP_
#define _CPU_HPP_

#include <cstdint>

class CPU
{
  // 12-bit register, using get/set methods to enforce 12-bit constraint.
  uint16_t address_register = 0;

public:
  /* clang-format off */
  typedef enum DataRegister_e
  {
    V0, V1, V2, V3, V4, V5, V6, V7, V8, V9, VA, VB, VC, VD, VE,
    VF, // doubles as carry/no-borrow/other flag
  } DataRegister_t;
  /* clang-format on */

  uint8_t registers[16] = { 0 }; // 8-bit data registers, V0 to VF.

  uint8_t sound_timer = 0;
  static DataRegister_t get_data_register (uint8_t x);
  void set_address_register (uint16_t addr);
  uint16_t get_address_register (void);
};

#endif
