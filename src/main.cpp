#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>

int
main (void)
{
  const std::string FILENAME = "helloworld.rom";
  std::ifstream file (FILENAME, std::ios::binary);
  if (!file)
    {
      std::cerr << "Error opening file: " << FILENAME << std::endl;
      return -1;
    }

  char byte;
  file.read (&byte, sizeof (byte));
  if (!file)
    {
      std::cerr << "Error reading byte from file.\n";
      return -1;
    }

  uint8_t byte_value = static_cast<uint8_t> (byte);
  if (file)
    {
      std::cout << "Byte: " << std::hex << std::setw (2) << std::setfill ('0')
                << static_cast<int> (byte_value) << std::endl;
    }
  else
    {
      std::cout << "Error reading from file.\n";
      return -1;
    }

  return 0;
}
