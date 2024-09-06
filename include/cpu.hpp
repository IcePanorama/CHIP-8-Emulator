#ifndef _CPU_HPP_
#define _CPU_HPP_

#include <array>
#include <cstdint>
#include <ostream>

class CPU
{
  /**
   *  12-bit address register (I). Using get/set methods to enforce the 12-bit
   *  constraint.
   */
  uint16_t address_register = 0;

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
};

std::ostream &operator<< (std::ostream &os, const CPU &c);

#endif
