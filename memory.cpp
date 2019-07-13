#include "base_definitions.h"
#include "memory.h"
#include "command.h"

std::vector<uchar> vm::Memory::get(pRegister pc, unsigned int size)
{
	std::vector<uchar> command;
	for (int i = 0; i < size; ++i) {
		command.push_back(m_buffer[pc + i]);
	}
	return command;
}


vm::Memory::Memory() {
	m_buffer = std::vector<uchar>(200);
}

