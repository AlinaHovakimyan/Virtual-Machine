#include <iostream>
#include "processor.h"
#include "declarations.h"
#include "command.h"
#include "executor.h"
#include <string>

vm::Processor::Processor(Memory& memory, size_t StackSize, int pc) :
	m_Memory(memory)
{
	if (&memory == nullptr)
		throw ExceptionError("Invalid Memory pointer.");
	if (memory.GetSize() < StackSize)
		throw ExceptionError("Failed to initialize processor: memory size is too small.");

	m_State = State();
	m_Executor = new Executor(&memory);
	m_State.m_StackLowerBound = (offset)(m_Memory.GetSize() - StackSize);
	m_State.m_StackUpperBound = (offset)m_Memory.GetSize();
	m_State.m_SP = m_State.m_StackUpperBound;
	m_State.m_SF = m_State.m_StackUpperBound;
	m_State.addressRegisters[6] = m_State.m_StackUpperBound;
	m_State.addressRegisters[7] = m_State.m_StackUpperBound;
	m_State.m_PC = pc;
}

int vm::Processor::run()
{

	bool successfull = true;
	try {
		if (m_Debugger != nullptr) {
			m_State.m_ContinueRun &= m_Debugger->Start();
		}

		while (m_State.m_ContinueRun) {
			// Fetch current command	
			m_State.m_Command = fetch();

			// for printing next commmand
			//m_Debugger->PrintNextCommand();

			// Decode current command
			exec execute = decode();


			// Increment program counter and go to the next command
			m_State.m_PC += m_State.m_CommandSize;

			// Check trap for debugger
			if (m_State.m_ContinueRun && m_State.m_flags.getTrap())
			{	// Break program executon and pass control to the debugger
				if (m_Debugger != nullptr) {
					m_State.m_ContinueRun = m_Debugger->Break();
				}
				else {
					m_State.m_ContinueRun = false;
				}
			}
			else {
				m_Debugger->checkBreak();
			}

			// Execute current command
			(this->m_Executor->*execute)(m_State);
		}
	}
	catch (ExceptionError exception) {
		std::cout << "Error: " << exception.message() << std::endl;
	}
	catch (...) {
		successfull = false;
		return 1;
	}

	if (m_Debugger != nullptr) {
		m_Debugger->End(successfull);
	}
	return 0;    // everything is ok
}

//------------------------------------------
// fetch command
//------------------------------------------

 pRegister vm::Processor::fetch()
 {
	 return *(pRegister*)(&(m_Memory)[m_State.m_PC]);
 }

//------------------------------------------
// decode command
//------------------------------------------

exec vm::Processor::decode()
{
	uchar* i = (uchar*)&m_State.m_Command;
	pRegister* com = (pRegister*)i;
	Command opCode = static_cast<Command>(*i);
	m_State.m_CommandSize = CommandParser::Commands.find(opCode)->second.commandSize;
	
	if (CommandParser::Commands.find(opCode) == CommandParser::Commands.end()) {
		throw ExceptionError("Command execute function can not be found");
	}
	return CommandParser::Commands.find(opCode)->second.execFunc;
}

//-------------------------------------------
// set processor state
//--------------------------------------------
void vm::Processor::SetState(State const& newState)
{
	m_State = newState;
}

vm::Processor::State::State()
{
	m_PC = 1;    // m_PC can't be zero   , later m_PC must be assigned to code section startPoint
	m_Command = 0;
	m_CommandSize = 0;
	m_ContinueRun = true;
	m_flags = Flags();
	m_SP = 0;
	m_SF = 0;
	m_StackLowerBound = 0;
	m_StackUpperBound = 0;
	std::memset(addressRegisters, 0, size_t(AddressRegistersSize * sizeof(uint)));
	std::memset(GPRegisters, 0, size_t(GPRegistersSize));
}


vm::Processor::State& vm::Processor::State::operator=(State const& state2)
{
	if (&state2 != this)
	{
		m_PC = state2.m_PC;
		m_Command = state2.m_Command;
		m_flags = state2.m_flags;

		// address registers count is 8 and each address register contains 4 bytes
		std::memcpy(addressRegisters, state2.addressRegisters, size_t(AddressRegistersSize * sizeof(uint)));
		
		// general purpos registers count is 32 and each of them contains 1 byte
		std::memcpy(GPRegisters, state2.GPRegisters, size_t(GPRegistersSize));
	
		m_SP = state2.m_SP;
		m_SF = state2.m_SF;
	}
	return *this;
}



vm::Processor::Flags::Flags()
	:m_flagsRegister(0)
{
}

//-------------------------------------------
// Processor::Flags Compare Functions
//-------------------------------------------

inline bool vm::Processor::Flags::IsEqual() const
{
	return false;
}

bool vm::Processor::Flags::IsNotEqual() const
{
	return false;
}

bool vm::Processor::Flags::IsAbove() const
{
	return false;
}

bool vm::Processor::Flags::IsAboveOrEqual() const
{
	return false;
}

bool vm::Processor::Flags::IsBelow() const
{
	return false;
}

bool vm::Processor::Flags::IsBelowOrEqual() const
{
	return false;
}

bool vm::Processor::Flags::IsGreat() const
{
	return false;
}

bool vm::Processor::Flags::IsGreatOrEqual() const
{
	return false;
}

bool vm::Processor::Flags::IsLow() const
{
	return false;
}

bool vm::Processor::Flags::IsLowOrEqual() const
{
	return false;
}

bool vm::Processor::Flags::IsOverflow() const
{
	return false;
}

bool vm::Processor::Flags::IsNotOverflow() const
{
	return false;
}

bool vm::Processor::Flags::IsSigned() const
{
	return false;
}

bool vm::Processor::Flags::IsNotSigned() const
{
	return false;
}

bool vm::Processor::Flags::IsParity() const
{
	return false;
}

bool vm::Processor::Flags::IsNotParity() const
{
	return false;
}

pRegister vm::Processor::Executor::getArgument(pRegister command, argIndex argumentIndex)
{
	int64_t argument = 0;
	int64_t* arg = NULL;
	uchar* i = (uchar*)&command;
	int count = 0;

	switch (argumentIndex) {
	case argIndex::First:
	{
		argument = *((uchar*)&command + 2);
		break;
	}
	case argIndex::Second:
	{
		argument = *((uchar*)&command + 3);
		break;
	}
	case argIndex::Third:
	{
		argument = *((uchar*)&command + 4);
		break;
	}
	}
	return argument;
}

void vm::Processor::AttachDebugger(IDebugger* Debugger)
{
	if (Debugger == nullptr) {
		throw ExceptionError("Failed to attach debugger: Invalid pointer.");
	}
	m_Debugger = Debugger;
	//m_State.m_flags.setTrap(true);
}

void vm::Processor::DetachDebugger()
{
	m_Debugger = nullptr;
}