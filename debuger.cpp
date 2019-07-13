#include "debugger.h"
#include "command.h"
#include <iostream>
#include <iomanip>

namespace vm {
	extern std::unordered_map<int, std::string> varFuncMap;
}

vm::Debugger::Debugger(Interpretator* Machine)
		: m_Machine(Machine), m_BreakCmd("BREAK")
{
}

bool vm::Debugger::Start()
{
	if (m_Machine != nullptr) {

		ProcessorPtr CPU = m_Machine->GetProcessor();
		CPU->GetFlags().setTrap(true);
		return true;
		//return Break();
	}
	return false;
}

void vm::Debugger::End(bool Successfull)
{
	if (Successfull) {
		std::cout << std::endl << ">> Program execution finished";
	}
	else {
		std::cout << std::endl << "!> Program execution stopped";
	}
	std::cin.get();

	m_Machine = nullptr;
}

bool vm::Debugger::Break()
{
	if (m_Machine != nullptr) {
		char key;
		ProcessorPtr CPU = m_Machine->GetProcessor();
		Processor::State const& State = CPU->GetState();
		if (State.m_PC > 0) {
			auto it = std::find(m_breakPtrs.begin(), m_breakPtrs.end(), State.m_PC);  // breakpoint
			if (it != m_breakPtrs.end()) {	// Breakpoint
				std::cout << "hit breakpoint" << std::endl;
				std::cin >> key;
				if (key == 'c')
				{	// Continue
					std::cout << "continue" << std::endl;
					CPU->GetFlags().setTrap(false);
					return true;
				}
			}
		}
		PrintState(State);
	} else {
			std::cout << std::endl << "!> Machine is not available";
	}
	while (m_Machine != nullptr) {
		ProcessorPtr CPU = m_Machine->GetProcessor();
		Processor::State const& State = CPU->GetState();
		std::cout << std::endl << "-> ";
		std::cin >> m_debugCommand;
		if (m_debugCommand == '?')
		{
			std::cout << "c                         continue: run program until finished or breakpoint found" << std::endl;
			std::cout << "s                         Step: execute single instruction" << std::endl;
			std::cout << "j <line>                  Jump: set PC to the specified number of source code" << std::endl;
			std::cout << "b <line>                  Set breakpoint at line number of source code" << std::endl;
			std::cout << "d <line>                  Remove breakpoint from line number of source code" << std::endl;
			std::cout << "q                         Stop program execution" << std::endl;
		}
		else if (m_debugCommand == 'j')
		{
			size_t commandNumber;
			std::cin >> commandNumber;
			if (true)
			{
				CPU->SetPC(commandNumber);
				PrintState(State);
			} else {
				std::cout << "!> Invalid source code line number.";
			}
		}
		else if (m_debugCommand == 'c')
		{
			auto it = std::find(m_breakPtrs.begin(), m_breakPtrs.end(), State.m_PC);
			if (it != m_breakPtrs.end()) {
				CPU->GetFlags().setTrap(true);
			}
			else {
				CPU->GetFlags().setTrap(false);
			}
			return true;
		}
		else if (m_debugCommand == 's')
		{
			CPU->GetFlags().setTrap(true);
			return true;
		}
		else if (m_debugCommand == 'b')
		{
			size_t breakPtr;
			std::cin >> breakPtr;
			if (std::find(m_breakPtrs.begin(), m_breakPtrs.end(), breakPtr) == m_breakPtrs.end())
			{
				std::cout << ">> Breakpoint set at PC line #" << std::dec << breakPtr;
				m_breakPtrs.push_back(breakPtr);
			}
			else
				std::cout << "!> Breakpoint not set.";
		}
		else if (m_debugCommand == 'd')
		{
			size_t breakPtr;
			std::cin >> breakPtr;
			auto it = std::find(m_breakPtrs.begin(), m_breakPtrs.end(), breakPtr);
			if (it != m_breakPtrs.end())
			{
				m_breakPtrs.erase(it);

				std::cout << ">> Breakpoint removed from PC line #" << std::dec << breakPtr;
			}
			else {
				std::cout << "!> Breakpoint not removed.";
			}
		}
		else if (m_debugCommand == 'q')
		{
			std::cout << "end debugging" << std::endl;
			m_Machine = nullptr;
		}
	}
	return false;
}

void vm::Debugger::checkBreak()
{
	auto it = std::find(m_breakPtrs.begin(), m_breakPtrs.end(), m_Machine->GetProcessor()->GetState().m_PC);
	if (m_debugCommand = 'c' && it != m_breakPtrs.end()) {
		m_Machine->GetProcessor()->GetState().m_flags.setTrap(true);
	}
}

void vm::Debugger::PrintState(Processor::State const& State)
{
	std::cout << std::endl;
	std::cout << "Processor status flags: ---------------------------------------"  << std::endl;
	std::cout << "CF:" << std::dec << State.m_flags.getCarry() << "  ";
	std::cout << "PF:" << std::dec << State.m_flags.getParity() << "  ";
	std::cout << "ZF:" << std::dec << State.m_flags.getZero() << "  ";
	std::cout << "SF:" << std::dec << State.m_flags.getSign() << "  ";
	std::cout << "OF:" << std::dec << State.m_flags.getOverflow();

	std::cout << std::endl;
	std::cout << "Address registers (0x): ---------------------------------------" << std::endl;
	std::cout << "A1 = " << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << State.addressRegisters[0] << "  ";
	std::cout << "A2 = " << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << State.addressRegisters[1] << "  ";
	std::cout << "A3 = " << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << State.addressRegisters[2] << "  ";
	std::cout << "A4 = " << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << State.addressRegisters[3] << "  ";
	std::cout << std::endl;
	std::cout << "A5 = " << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << State.addressRegisters[4] << "  ";
	std::cout << "A6 = " << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << State.addressRegisters[5] << "  ";
	std::cout << "A7 (SP)= " << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << State.addressRegisters[6] << "  ";
	std::cout << "A8 (SF)= " << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << State.addressRegisters[7] << "  ";

	std::cout << std::endl;
	std::cout << "General purpose registers: --------------------------------------------------------------" << std::endl;
	
	for (int i = 0; i < Processor::GPRegistersSize / 2; ++i) {
		std::cout << "R" << i << "=" << std::dec << State.GPRegisters[i] << "  ";
	}
	std::cout << std::endl;
	for (int i = Processor::GPRegistersSize / 2 + 1; i < Processor::GPRegistersSize; ++i) {
		std::cout << "R" << i << "=" << std::dec << State.GPRegisters[i] << "  ";
	}
	std::cout << std::endl;
	PrintNextCommand();
	//std::cout << "Next command: --------------------------------------------------------------------------------" << std::endl;
	//std::cout << "PC = " << std::dec << State.m_PC << ":  " << CommandParser::CommandNames.find(static_cast<Command>(*(reinterpret_cast<uint8*>(State.m_Command))))->second;
}

void vm::Debugger::PrintNextCommand() {
	std::cout << "Next command: --------------------------------------------------------------------------------" << std::endl;
	Command com = static_cast<Command>(*(reinterpret_cast<uint8*>(&m_Machine->GetProcessor()->GetState().m_Command)));
	std::cout << "PC = " << std::dec << m_Machine->GetProcessor()->GetState().m_PC << ":  " << CommandParser::CommandNames.find(com)->second;
	if (CommandParser::Commands.find(com)->second.commandSize == 1) {
		return;
	}
	else {
		size Size = static_cast<size>(*(reinterpret_cast<uint8*>(&m_Machine->GetProcessor()->GetState().m_Command) + 1));
		std::cout << " " << CommandParser::sizeName.find(Size)->second;
	}
	if (CommandParser::Commands.find(com)->second.commandSize == 3) {
		Register reg = static_cast<Register>(*(reinterpret_cast<uint8*>(&m_Machine->GetProcessor()->GetState().m_Command) + 2));
		std::cout << " "<< CommandParser::RegisterName.find(reg)->second;
	}
	else if (CommandParser::Commands.find(com)->second.commandSize == 4) {
		Register reg = static_cast<Register>(*(reinterpret_cast<uint8*>(&m_Machine->GetProcessor()->GetState().m_Command) + 2));
		std::cout << " " << CommandParser::RegisterName.find(reg)->second;
		reg = static_cast<Register>(*(reinterpret_cast<uint8*>(&m_Machine->GetProcessor()->GetState().m_Command) + 3));
		std::cout << " " << CommandParser::RegisterName.find(reg)->second;
	}
	else if (CommandParser::Commands.find(com)->second.commandSize == 5) {
		Register reg = static_cast<Register>(*(reinterpret_cast<uint8*>(&m_Machine->GetProcessor()->GetState().m_Command) + 2));
		std::cout << " " << CommandParser::RegisterName.find(reg)->second;
		reg = static_cast<Register>(*(reinterpret_cast<uint8*>(&m_Machine->GetProcessor()->GetState().m_Command) + 3));
		std::cout << " " << CommandParser::RegisterName.find(reg)->second;
		reg = static_cast<Register>(*(reinterpret_cast<uint8*>(&m_Machine->GetProcessor()->GetState().m_Command) + 4));
		std::cout << " " << CommandParser::RegisterName.find(reg)->second;
	}
	else if (CommandParser::Commands.find(com)->second.commandSize == 7) {
		Register reg = static_cast<Register>(*(reinterpret_cast<uint8*>(&m_Machine->GetProcessor()->GetState().m_Command) + 2));
		std::cout << " " << CommandParser::RegisterName.find(reg)->second;
		if (com == Command::ASSIGNADDRESS) {
			std::cout << " " << varFuncMap.find(static_cast<uint32>(*(reinterpret_cast<uint8*>(&m_Machine->GetProcessor()->GetState().m_Command) + 3)))->second;
		}
		else {
			std::cout << " " << std::dec << static_cast<uint32>(*(reinterpret_cast<uint8*>(&m_Machine->GetProcessor()->GetState().m_Command) + 3));
		}
	}
	std::cout << std::endl;
}