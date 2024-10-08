#include "utils.hpp"

uint8_t
get_uint8 (std::ifstream &fptr)
{
  char byte;
  fptr.read (&byte, sizeof (byte));

  return static_cast<uint8_t> (byte);
}

uint16_t
get_uint16 (std::ifstream &fptr)
{
  uint8_t low_byte = get_uint8 (fptr);
  uint8_t high_byte = get_uint8 (fptr);
  return (static_cast<uint16_t> (low_byte) << 8) | high_byte;
}
