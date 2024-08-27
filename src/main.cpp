#include "cpu.hpp"
#include "sprite.hpp"

#include <array>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <format>
#include <fstream>
#include <iostream>
#include <stdexcept>

std::array<Sprite, 16> global_sprites;

uint8_t get_byte (std::ifstream &fptr);
uint16_t get_uint16 (std::ifstream &fptr);
void handle_set_data_register_to_nn_opcode (CPU &cpu, uint16_t input);
void handle_set_address_register_opcode (CPU &cpu, uint16_t input);
void handle_get_key_opcode (CPU &cpu, uint16_t input);
void handle_clear_display_opcode (void);
void handle_sound_timer_opcode (CPU &cpu, uint16_t input);
void init_global_sprites (void);
void handle_set_addr_reg_to_loc_of_sprite_opcode (CPU &cpu, uint16_t input);
void handle_draw_opcode (CPU &cpu, uint16_t input);

int
main (void)
{
  init_global_sprites ();

  const std::string FILENAME = "helloworld.rom";
  std::ifstream file (FILENAME, std::ios::binary);
  if (!file)
    {
      std::cerr << "Error opening file: " << FILENAME << std::endl;
      return -1;
    }
  CPU cpu;

  while (true)
    {
      uint16_t opcode = get_uint16 (file);
      if (file.eof ())
        break;

      std::cout << std::format ("Opcode: {:04X}\n", opcode);

      switch (uint8_t (opcode >> 12))
        {
        case 0x0:
          if (opcode == 0x00E0)
            {
              handle_clear_display_opcode ();
              break;
            }
          [[fallthrough]];
        case 0x6:
          handle_set_data_register_to_nn_opcode (cpu, opcode);
          break;
        case 0xA:
          handle_set_address_register_opcode (cpu, opcode);
          break;
        case 0xD:
          handle_draw_opcode (cpu, opcode);
          break;
        case 0xF:
          if ((opcode & 0xFF) == 0x0A)
            {
              handle_get_key_opcode (cpu, opcode);
              break;
            }
          else if ((opcode & 0xFF) == 0x18)
            {
              handle_sound_timer_opcode (cpu, opcode);
              break;
            }
          else if ((opcode & 0xFF) == 0x29)
            {
              handle_set_addr_reg_to_loc_of_sprite_opcode (cpu, opcode);
              break;
            }
          [[fallthrough]];
        default:
          std::cout << std::format ("Unknown opcode: {:04X}\n", opcode);
        }

      std::cout << std::endl;
    }

  return 0;
}

uint8_t
get_byte (std::ifstream &fptr)
{
  char byte;
  fptr.read (&byte, sizeof (byte));

  return static_cast<uint8_t> (byte);
}

uint16_t
get_uint16 (std::ifstream &fptr)
{
  uint8_t low_byte = get_byte (fptr);
  uint8_t high_byte = get_byte (fptr);
  return (static_cast<uint16_t> (low_byte) << 8) | high_byte;
}

void
handle_set_data_register_to_nn_opcode (CPU &cpu, uint16_t input)
{
  CPU::data_register reg = CPU::get_data_register ((input >> 8) & 0xF);
  std::cout << std::format ("Register {} before: {:02X}\n",
                            static_cast<int> (reg), cpu.registers[reg]);

  cpu.registers[reg] = input & 0xFF;
  std::cout << std::format ("Register {} after: {:02X}\n",
                            static_cast<int> (reg), cpu.registers[reg]);
}

void
handle_set_address_register_opcode (CPU &cpu, uint16_t input)
{
  std::cout << std::format ("Before: {:03X}\n", cpu.get_address_register ());
  cpu.set_address_register (input & 0xFFF);
  std::cout << std::format ("After: {:03X}\n", cpu.get_address_register ());
}

void
handle_get_key_opcode (CPU &cpu, uint16_t input)
{
  // User must currently press enter for keycode to be registered
  // This'll be changed when I switch to using raylib or whatever.
  char c = getchar ();
  uint8_t keycode = 0;

  if (c >= 'a' && c <= 'f')
    keycode = c - 'a' + 10;
  else if (c >= 'A' && c <= 'F')
    keycode = c - 'A' + 10;
  else if (c >= '0' && c <= '9')
    keycode = c - '0';
  else
    throw std::runtime_error (std::format ("Invalid key, %c.\n", c));

  std::cout << std::format ("keycode: {:02X}.\n", keycode);

  CPU::data_register reg = CPU::get_data_register ((input >> 8) & 0xF);
  std::cout << std::format ("Register {} before: {:02X}\n",
                            static_cast<int> (reg), cpu.registers[reg]);
  cpu.registers[reg] = keycode;
  std::cout << std::format ("Register {} after: {:02X}\n",
                            static_cast<int> (reg), cpu.registers[reg]);
}

void
handle_clear_display_opcode (void)
{
  std::cout << "Clearing the screen...\n";
}

void
handle_sound_timer_opcode (CPU &cpu, uint16_t input)
{
  CPU::data_register reg = CPU::get_data_register ((input >> 8) & 0xF);
  std::cout << std::format ("Sound timer before: {:02X}\n", cpu.sound_timer);
  cpu.sound_timer = cpu.registers[reg];
  std::cout << std::format ("Sound timer after: {:02X}\n", cpu.sound_timer);
}

/* See: http://devernay.free.fr/hacks/chip8/C8TECH10.HTM */
void
init_global_sprites (void)
{
  /* clang-format off */
  global_sprites = {{
    Sprite (0x0, 0x0 + 5 * 0, { 0xF0, 0x90, 0x90, 0x90, 0xF0 }),
    Sprite (0x1, 0x0 + 5 * 1, { 0x20, 0x60, 0x20, 0x20, 0x70 }),
    Sprite (0x2, 0x0 + 5 * 2, { 0xF0, 0x10, 0xF0, 0x80, 0xF0 }),
    Sprite (0x3, 0x0 + 5 * 3, { 0xF0, 0x10, 0xF0, 0x10, 0xF0 }),
    Sprite (0x4, 0x0 + 5 * 4, { 0x90, 0x90, 0xF0, 0x10, 0x10 }),
    Sprite (0x5, 0x0 + 5 * 5, { 0xF0, 0x80, 0xF0, 0x10, 0xF0 }),
    Sprite (0x6, 0x0 + 5 * 6, { 0xF0, 0x80, 0xF0, 0x90, 0xF0 }),
    Sprite (0x7, 0x0 + 5 * 7, { 0xF0, 0x10, 0x20, 0x40, 0x40 }),
    Sprite (0x8, 0x0 + 5 * 8, { 0xF0, 0x90, 0xF0, 0x90, 0xF0 }),
    Sprite (0x9, 0x0 + 5 * 9, { 0xF0, 0x90, 0xF0, 0x10, 0xF0 }),
    Sprite (0xA, 0x0 + 5 * 10, { 0xF0, 0x90, 0xF0, 0x90, 0x90 }),
    Sprite (0xB, 0x0 + 5 * 11, { 0xE0, 0x90, 0xE0, 0x90, 0xE0 }),
    Sprite (0xC, 0x0 + 5 * 12, { 0xF0, 0x80, 0x80, 0x80, 0xF0 }),
    Sprite (0xD, 0x0 + 5 * 13, { 0xE0, 0x90, 0x90, 0x90, 0xE0 }),
    Sprite (0xE, 0x0 + 5 * 14, { 0xF0, 0x80, 0xF0, 0x80, 0xF0 }),
    Sprite (0xF, 0x0 + 5 * 15, { 0xF0, 0x80, 0xF0, 0x80, 0x80 })
  }};
  /* clang-format on */
}

void
handle_set_addr_reg_to_loc_of_sprite_opcode (CPU &cpu, uint16_t input)
{
  CPU::data_register reg = CPU::get_data_register ((input >> 8) & 0xF);

  Sprite *sptr = nullptr;
  for (uint8_t i = 0; i < global_sprites.size (); i++)
    {
      if (global_sprites.at (i).character == cpu.registers[reg])
        {
          sptr = &global_sprites.at (i);
          break;
        }
    }

  if (sptr == nullptr)
    throw std::runtime_error (
        std::format ("Could not locate sprite for character, {:02X}\n",
                     cpu.registers[reg]));

  std::cout << std::format ("Before: {:03X}\n", cpu.get_address_register ());
  cpu.set_address_register (sptr->location_in_memory);
  std::cout << std::format ("After: {:03X}\n", cpu.get_address_register ());
}

void
handle_draw_opcode (CPU &cpu, uint16_t input)
{
  CPU::data_register x_pos_reg = CPU::get_data_register ((input >> 8) & 0xF);
  CPU::data_register y_pos_reg = CPU::get_data_register ((input >> 4) & 0xF);

  uint8_t x_pos = cpu.registers[x_pos_reg];
  uint8_t y_pos = cpu.registers[y_pos_reg];
  std::cout << std::format ("Draw coords: ({:02X},{:02X})\n", x_pos, y_pos);
}
