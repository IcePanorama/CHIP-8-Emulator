#ifndef _CPU_HPP_
#define _CPU_HPP_

#include <cstdint>

class CPU {
	// 12-bit register, using uint16_t for simplicity.
	uint16_t memory_address_register = 0;
public:
	typedef enum data_register {
		V0,
		V1,
		V2,
		V3,
		V4,
		V5,
		V6,
		V7,
		V8,
		V9,
		VA,
		VB,
		VC,
		VD,
		VE,
		VF,	// doubles as carry/no-borrow/other flag
	} data_register;
	
	uint8_t registers[16] = {0};	// 8-bit data registers, V0 to VF.
	
	static data_register get_data_register (uint8_t x);
	void set_memory_address_register (uint16_t addr);
};

#endif
