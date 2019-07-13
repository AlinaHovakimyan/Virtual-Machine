#ifndef COMMAND_H
#define COMMAND_H

#include "processor.h"
#include <string>
#include <map>
#include <unordered_map>

enum Command {
	NOP = 0b00000000,
	JUMPOFFSET = 0b00000001,
	JUMPADDRESS = 0b00000010,
	ASSIGNADDRESS = 0b00000011,
	ASSIGNNUMBER =  0b00000100,
	MOVE = 0b00000101,
	SWAP = 0b00000110,
	AND = 0b00000111,
	OR = 0b00001000,
	XOR = 0b00001001,
	NOT = 0b00001010,
	NAND = 0b00001011,
	NOR = 0b00001100,
	ADD = 0b00001101,
	SUB = 0b00001110,
	MUL = 0b00001111,
	DIV = 0b00010000,
	INC = 0b00010001,
	DEC = 0b00010010,
	EXIT = 0b00010011,
	CALL = 0b00010100,
	PUSH = 0b00010101,
	POP  =  0b00010110,
	PUSHF = 0b00010111,
	POPF =  0b00011000,
	RET  =  0b00011001,
	LOAD = 0b00011011,
	STORE = 0b00011100,
	BREAK = 0b00011101
};


typedef void(vm::Processor::Executor::*exec)(vm::Processor::State&);

struct pair {
	exec execFunc;
	unsigned int commandSize;
	pair(exec funcP, unsigned int size) :
		execFunc(funcP), commandSize(size)
	{};
};

namespace vm {
	enum Register {
		R0 = 0b00000000,
		R1 = 0b00000001,
		R2 = 0b00000010,
		R3 = 0b00000011,
		R4 = 0b00000100,
		R5 = 0b00000101,
		R6 = 0b00000110,
		R7 = 0b00000111,
		R8 = 0b00001000,
		R9 = 0b00001001,
		R10 = 0b00001010,
		R11 = 0b00001011,
		R12 = 0b00001100,
		R13 = 0b00001101,
		R14 = 0b00001110,
		R15 = 0b00001111,
		R16 = 0b00010000,
		R17 = 0b00010001,
		R18 = 0b00010010,
		R19 = 0b00010011,
		R20 = 0b00010100,
		R21 = 0b00010101,
		R22 = 0b00010110,
		R23 = 0b00010111,
		R24 = 0b00011000,
		R25 = 0b00011001,
		R26 = 0b00011010,
		R27 = 0b00011011,
		R28 = 0b00011100,
		R29 = 0b00011101,
		R30 = 0b00011110,
		R31 = 0b00011111,
		R32 = 0b00100000,
		R33 = 0b00100001,
		R34 = 0b00100010,
		R35 = 0b00100011,
		R36 = 0b00100100,
		R37 = 0b00100101,
		R38 = 0b00100110,
		R39 = 0b00100111,
		R40 = 0b00101000,
		R41 = 0b00101001,
		R42 = 0b00101010,
		R43 = 0b00101011,
		R44 = 0b00101100,
		R45 = 0b00101101,
		R46 = 0b00101110,
		R47 = 0b00101111,
		R48 = 0b00110000,
		R49 = 0b00110001,
		R50 = 0b00110010,
		R51 = 0b00110011,
		R52 = 0b00110100,
		R53 = 0b00110101,
		R54 = 0b00110110,
		R55 = 0b00110111,
		R56 = 0b00111000,
		R57 = 0b00111001,
		R58 = 0b00111010,
		R59 = 0b00111011,
		R60 = 0b00111100,
		R61 = 0b00111101,
		R62 = 0b00111110,
		R63 = 0b00111111,
		A1 = 0b01000000,
		A2 = 0b01000001,
		A3 = 0b01000010,
		A4 = 0b01000011,
		A5 = 0b01000100,
		A6 = 0b01000101,
		SF = 0b01000110,
		SP = 0b01000111,	
		A7 = 0b01000110,
		A8 = 0b01000111,
	};
	enum size {
		DWord = 0b00,
		Byte = 0b01,
		Word = 0b10,
		QWord = 0b11
	};
	class CommandParser {
	public:
		static const std::unordered_map<Command, pair> Commands;
		static const std::map<std::string, Command> CommandOpCodes;
		static const std::map<Command, std::string> CommandNames;
		static const std::map<Register, std::string> RegisterName;
		static const std::map<size, std::string> sizeName;
	};
}

#define addresRegZero 64
#endif
