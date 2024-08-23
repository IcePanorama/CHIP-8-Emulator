#ifndef _SPRITE_HPP_
#define _SPRITE_HPP_

#include <array>
#include <cstdint>

class Sprite
{
public:
  uint8_t character;           // character associated with said sprite
  uint16_t location_in_memory; // somewhere between 0x0 to 0x1FF
  std::array<uint8_t, 5> data; // the actual sprite data itself
};

#endif
