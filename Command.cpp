#include "command.h"

namespace vm {
	typedef std::map<int16, exec> funcMap;

	const std::unordered_map<Command, pair> vm::CommandParser::Commands {
		{ Command::NOP, pair(&Processor::Executor::Nop, 1) },
		{ Command::JUMPOFFSET, pair(&Processor::Executor::JumpOffset, 7) },
		{ Command::JUMPADDRESS, pair(&Processor::Executor::JumpAddress, 3) },
		{ Command::ASSIGNADDRESS, pair(&Processor::Executor::AssignAdress, 7) },
		{ Command::ASSIGNNUMBER, pair(&Processor::Executor::AssignNumber, 7) },
		{ Command::MOVE, pair(&Processor::Executor::Move, 4) },
		{ Command::SWAP, pair(&Processor::Executor::Swap, 4) },
		{ Command::AND, pair(&Processor::Executor::And, 5) },
		{ Command::OR, pair(&Processor::Executor::Or, 5) },
		{ Command::XOR, pair(&Processor::Executor::Xor, 5) },
		{ Command::NOT, pair(&Processor::Executor::Not, 3) },
		{ Command::NAND, pair(&Processor::Executor::Nand, 5) },
		{ Command::NOR, pair(&Processor::Executor::Nor, 5) },
		{ Command::ADD, pair(&Processor::Executor::Add, 5) },
		{ Command::SUB, pair(&Processor::Executor::Sub, 5) },
		{ Command::MUL, pair(&Processor::Executor::Mul, 5) },
		{ Command::DIV, pair(&Processor::Executor::Div, 5) },
		{ Command::INC, pair(&Processor::Executor::Inc, 3) },
		{ Command::DEC, pair(&Processor::Executor::Dec, 3) },
		{ Command::EXIT, pair(&Processor::Executor::Exit, 1) },
		{ Command::CALL, pair(&Processor::Executor::Call, 3) },
		{ Command::PUSH, pair(&Processor::Executor::Push, 3) },
		{ Command::POP, pair(&Processor::Executor::Pop, 3) },
	    {Command::PUSHF, pair(&Processor::Executor::PushF, 1)},
	    { Command::POPF, pair(&Processor::Executor::PopF, 1) },
		{ Command::RET, pair(&Processor::Executor::Ret, 1) },
		{ Command::LOAD, pair(&Processor::Executor::Load, 4) },
		{ Command::STORE, pair(&Processor::Executor::Store, 4) },
		{ Command::BREAK, pair(&Processor::Executor::Break, 1)}
		};

};


const std::map<std::string, Command> vm::CommandParser::CommandOpCodes  {
	{ "NOP", Command::NOP },
{ "JUMPOFFSET", Command::JUMPOFFSET },
{ "JUMPADDRESS", Command::JUMPADDRESS },
{ "ASSIGNADDRESS", Command::ASSIGNADDRESS },
{ "ASSIGNNUMBER", Command::ASSIGNNUMBER },
{ "MOVE", Command::MOVE },
{ "SWAP", Command::SWAP },
{ "AND", Command::AND },
{ "OR", Command::OR },
{ "XOR", Command::XOR },
{ "NOT", Command::NOT },
{ "NAND",Command::NAND },
{ "NOR", Command::NOR },
{ "ADD", Command::ADD },
{ "SUB", Command::SUB },
{ "MUL", Command::MUL },
{ "DIV", Command::DIV },
{ "INC", Command::INC },
{ "DEC", Command::DEC },
{ "EXIT",Command::EXIT },
{ "CALL",Command::CALL },
{ "PUSH",Command::PUSH },
{ "POP", Command::POP },
{ "RET", Command::RET },
{ "LOAD",Command::LOAD },
{ "STORE", Command::STORE },
};


const std::map<Command, std::string> vm::CommandParser::CommandNames {
	{ Command::NOP, "NOP" },
{ Command::JUMPOFFSET,  "JUMPOFFSET", },
{ Command::JUMPADDRESS, "JUMPADDRESS" },
{ Command::ASSIGNADDRESS, "ASSIGNADDRESS" },
{ Command::ASSIGNNUMBER, "ASSIGNNUMBER" },
{ Command::MOVE, "MOVE" },
{ Command::SWAP,  "SWAP" },
{ Command::AND,  "AND" },
{ Command::OR,  "OR" },
{ Command::XOR,  "XOR" },
{ Command::NOT,  "NOT" },
{ Command::NAND, "NAND" },
{ Command::NOR, "NOR" },
{ Command::ADD, "ADD" },
{ Command::SUB, "SUB" },
{ Command::MUL, "MUL" },
{ Command::DIV, "DIV" },
{ Command::INC, "INC" },
{ Command::DEC, "DEC" },
{ Command::EXIT, "EXIT" },
{ Command::CALL, "CALL" },
{ Command::PUSH, "PUSH" },
{ Command::POP, "POP" },
{ Command::RET, "RET" },
{ Command::LOAD, "LOAD" },
{ Command::STORE, "STORE" },
};

const std::map<vm::Register, std::string> vm::CommandParser::RegisterName {
	{ Register::R0, "R0"},
	{ Register::R1, "R1"},
	{ Register::R2, "R2"},
	{ Register::R3, "R3"},
	{ Register::R4, "R4"},
	{ Register::R5, "R5"},
	{ Register::R6, "R6"},
	{ Register::R7, "R7"},
	{ Register::R8, "R8"},
	{ Register::R9, "R9"},
	{ Register::R10, "R10"},
	{ Register::R11, "R11"},
	{ Register::R12, "R12"},
	{ Register::R13, "R13"},
	{ Register::R14, "R14"},
	{ Register::R15, "R15"},
	{ Register::R16, "R16"},
	{ Register::R17, "R17"},
	{ Register::R18, "R18"},
	{ Register::R19, "R19"},
	{ Register::R20, "R20"},
	{ Register::R21, "R21"},
	{ Register::R22, "R22"},
	{ Register::R23, "R23"},
	{ Register::R24, "R24"},
	{ Register::R25, "R25"},
	{ Register::R26, "R26"},
	{ Register::R27, "R27"},
	{ Register::R28, "R28"},
	{ Register::R29, "R29"},
	{ Register::R30, "R30"},
	{ Register::R31, "R31"},
	{ Register::R32, "R32"},
	{ Register::R33, "R33"},
	{ Register::R34, "R34"},
	{ Register::R35, "R35"},
	{ Register::R36, "R36"},
	{ Register::R37, "R37"},
	{ Register::R38, "R38"},
	{ Register::R39, "R39"},
	{ Register::R40, "R40"},
	{ Register::R41, "R41"},
	{ Register::R42, "R42"},
	{ Register::R43, "R43"},
	{ Register::R44, "R44"},
	{ Register::R45, "R45"},
	{ Register::R46, "R46"},
	{ Register::R47, "R47"},
	{ Register::R48, "R48"},
	{ Register::R49, "R49"},
	{ Register::R50, "R50"},
	{ Register::R51, "R51"},
	{ Register::R52, "R52"},
	{ Register::R53, "R53"},
	{ Register::R54, "R54"},
	{ Register::R55, "R55"},
	{ Register::R56, "R56"},
	{ Register::R57, "R57"},
	{ Register::R58, "R58"},
	{ Register::R59, "R59"},
	{ Register::R60, "R60"},
	{ Register::R61, "R61"},
	{ Register::R62, "R62"},
	{ Register::R63, "R63"},
	{ Register::A1, "A1"},
	{ Register::A2, "A2"},
	{ Register::A3, "A3"},
	{ Register::A4, "A4"},
	{ Register::A5, "A5"},
	{ Register::A6, "A6"},
	{ Register::SF, "SF" },
	{ Register::SP, "SP" },
	{ Register::A7, "A7"},
	{ Register::A8, "A8"},
};

const std::map<vm::size, std::string> vm::CommandParser::sizeName{
	{ size::DWord, "DW"},
	{ size::Byte, "B"},
	{ size::Word, "W"},
	{ size::QWord, "QW"},
};