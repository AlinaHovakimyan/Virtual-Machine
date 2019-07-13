#include "executor.h"
#include "declarations.h"
#include "command.h"
#include "asmFunctions.h"
#include <bitset>

vm::Processor::Executor::Executor(MemoryPtr memory)
                         :m_Memory(memory)
{
	if (m_Memory == nullptr)
		throw ExceptionError("Failed to initialize IA32 Executor: Invalid argument.");
}

void vm::Processor::Executor::Nop(State& vmState)
{
	//no operation
}

void vm::Processor::Executor::Exit(State& vmState)
{
	vmState.m_ContinueRun = false;
}

void vm::Processor::Executor::Break(State& vmState)
{
	vmState.m_flags.setTrap(true);
}

void vm::Processor::Executor::JumpOffset(State & vmState)
{
	int32_t firstArg = getArgument(vmState.m_Command, argIndex::First);
	int size = *((uchar*)&vmState.m_Command + 1);
	switch (size)
	{
	case vm::size::Byte:
	{
		vmState.m_PC += static_cast<uint8>(firstArg);
		break;
	}
	case vm::size::Word:
	{
		vmState.m_PC += static_cast<uint16>(firstArg);
		break;
	}
	case vm::size::DWord:
	{
		vmState.m_PC += static_cast<uint32>(firstArg);
		break;
	}
	case vm::size::QWord:
	{
		vmState.m_PC += static_cast<uint64>(firstArg);
		break;
	}
	default:
		throw ExceptionError("Size is incorrect.");
	}
}

void vm::Processor::Executor::JumpAddress(State & vmState)
{
	int32_t firstArg = getArgument(vmState.m_Command, argIndex::First);
	int size = *((uchar*)&vmState.m_Command + 1);
	switch (size)
	{
	case vm::size::Byte:
	{
		vmState.m_PC = *reinterpret_cast<uint8*>(&vmState.addressRegisters[firstArg]);
		break;
	}
	case vm::size::Word:
	{
		vmState.m_PC = *reinterpret_cast<uint16*>(&vmState.addressRegisters[firstArg]);
		break;
	}
	case vm::size::DWord:
	{
		vmState.m_PC = *reinterpret_cast<uint32*>(&vmState.addressRegisters[firstArg]);
		break;
	}
	case vm::size::QWord:
	{
		vmState.m_PC = *reinterpret_cast<uint64*>(&vmState.addressRegisters[firstArg]);
		break;
	}
	default:
		throw ExceptionError("Size is incorrect.");
	}
}

void vm::Processor::Executor::Call(State & vmState)
{
	int32_t funcAddressReg = getArgument(vmState.m_Command, argIndex::First);

	// Push PC
    vmState.m_SP -= sizeof(size_t);
	if (vmState.m_SP < vmState.m_StackLowerBound)
		throw ExceptionError("Runtime error in Call: Stack overflow.");
	m_Memory->At<size_t>(vmState.m_SP) = vmState.m_PC;

	// Push SF
	vmState.m_SP -= sizeof(size_t);
	if (vmState.m_SP < vmState.m_StackLowerBound)
		throw ExceptionError("Runtime error in Call: Stack overflow.");
	m_Memory->At<size_t>(vmState.m_SP) = vmState.m_SF;

	// Change PC
	vmState.m_PC = (size_t)*(&vmState.addressRegisters[funcAddressReg - addresRegZero]);
	// Change SF
	vmState.m_SF = vmState.m_SP;
}

void vm::Processor::Executor::Ret(State & vmState)
{
	// Pop SF
	if ((vmState.m_SP + int(sizeof(size_t))) > vmState.m_StackUpperBound)
		throw ExceptionError("Runtime error in Call: Stack underflow.");
	vmState.m_SF = m_Memory->At<int>(vmState.m_SP);
	vmState.m_SP += sizeof(size_t);

	// Pop PC
	if ((vmState.m_SP + int(sizeof(size_t))) >  vmState.m_StackUpperBound)
		throw ExceptionError("Runtime error in Call: Stack underflow.");
	vmState.m_PC = m_Memory->At<size_t>(vmState.m_SP);
	vmState.m_SP += sizeof(size_t);
}

void vm::Processor::Executor::AssignNumber(State & vmState)
{
	// Get register
	int64_t firstArg = getArgument(vmState.m_Command, argIndex::First);
	int64_t secondArg = getArgument(vmState.m_Command, argIndex::Second);

	int size = *((uchar*)&vmState.m_Command + 1);

	switch (size)
	{
	case vm::size::Byte :
	{
		*reinterpret_cast<uint8*>(&vmState.GPRegisters[firstArg]) = static_cast<uint8>(secondArg);
		break;
	}
	case vm::size::Word :
	{
		*reinterpret_cast<uint16*>(&vmState.GPRegisters[firstArg]) = static_cast<uint16>(secondArg);
		break;
	}
	case vm::size::DWord:
	{
		*reinterpret_cast<uint32*>(&vmState.GPRegisters[firstArg]) = static_cast<uint32>(secondArg);
		break;
	}
	case vm::size::QWord:
	{
		*reinterpret_cast<uint64*>(&vmState.GPRegisters[firstArg]) = static_cast<uint64>(secondArg);
		break;
	}
	default:
		throw ExceptionError("Size is incorrect.");
	}
}

void vm::Processor::Executor::AssignAdress(State& vmState)
{
	// Get register
	int32_t firstArg = getArgument(vmState.m_Command, argIndex::First);
	int32_t secondArg = getArgument(vmState.m_Command, argIndex::Second);
	vmState.addressRegisters[firstArg - 64] = secondArg;
}


void vm::Processor::Executor::Move(State & vmState)
{
	int32_t firstArg = getArgument(vmState.m_Command, argIndex::First);
	int32_t secondArg = getArgument(vmState.m_Command, argIndex::Second);

	int size = *((uchar*)&vmState.m_Command + 1);
	switch (size)
	{
	case vm::size::Byte:
	{
		*reinterpret_cast<uint8*>(&vmState.GPRegisters[firstArg]) = *reinterpret_cast<uint8*>(&vmState.GPRegisters[secondArg]);
		break;
	}
	case vm::size::Word:
	{
		*reinterpret_cast<uint16*>(&vmState.GPRegisters[firstArg]) = *reinterpret_cast<uint16*>(&vmState.GPRegisters[secondArg]);
		break;
	}
	case vm::size::DWord:
	{
		*reinterpret_cast<uint32*>(&vmState.GPRegisters[firstArg]) = *reinterpret_cast<uint32*>(&vmState.GPRegisters[secondArg]);
		break;
	}
	case vm::size::QWord:
	{
		*reinterpret_cast<uint64*>(&vmState.GPRegisters[firstArg]) = *reinterpret_cast<uint64*>(&vmState.GPRegisters[secondArg]);
		break;
	}
	default:
		throw ExceptionError("Size is incorrect.");
	}
}

void vm::Processor::Executor::Swap(State & vmState)
{
	int32_t firstArg = getArgument(vmState.m_Command, argIndex::First);
	int32_t secondArg = getArgument(vmState.m_Command, argIndex::Second);

	int size = *((uchar*)&vmState.m_Command + 1);
	switch (size)
	{
	case vm::size::Byte:
	{
		std::swap(*reinterpret_cast<uint8*>(&vmState.GPRegisters[firstArg]), *reinterpret_cast<uint8*>(&vmState.GPRegisters[secondArg]));
        break;
	}
	case vm::size::Word:
	{
		std::swap(*reinterpret_cast<uint16*>(&vmState.GPRegisters[firstArg]), *reinterpret_cast<uint16*>(&vmState.GPRegisters[secondArg]));
		break;
	}
	case vm::size::DWord:
	{
		std::swap(*reinterpret_cast<uint32*>(&vmState.GPRegisters[firstArg]), *reinterpret_cast<uint32*>(&vmState.GPRegisters[secondArg]));
		break;
	}
	case vm::size::QWord:
	{
		std::swap(*reinterpret_cast<uint64*>(&vmState.GPRegisters[firstArg]) , *reinterpret_cast<uint64*>(&vmState.GPRegisters[secondArg]));
		break;
	}
	default:
		throw ExceptionError("Size is incorrect.");
	}
}

void vm::Processor::Executor::And(State & vmState)
{
	int size = *((uchar*)&vmState.m_Command + 1);

	// first operand
	pRegister firstArg = getArgument(vmState.m_Command, argIndex::First);
	// second operand
	pRegister secondArg = getArgument(vmState.m_Command, argIndex::Second);
	// Get result operand
	pRegister thirdArg = getArgument(vmState.m_Command, argIndex::Third);

	int16 flags;
	switch (size)
	{
	case vm::size::Byte:
	{
		flags = asmAND8(reinterpret_cast<uint8*>(&vmState.GPRegisters[firstArg]), reinterpret_cast<uint8*>(&vmState.GPRegisters[secondArg]), reinterpret_cast<uint8*>(&vmState.GPRegisters[thirdArg]));
		break;
	}
	case vm::size::Word:
	{
		flags = asmAND16(reinterpret_cast<uint16*>(&vmState.GPRegisters[firstArg]), reinterpret_cast<uint16*>(&vmState.GPRegisters[secondArg]), reinterpret_cast<uint16*>(&vmState.GPRegisters[thirdArg]));
		break;
	}
	case vm::size::DWord:
	{
		flags = asmAND32(reinterpret_cast<uint32*>(&vmState.GPRegisters[firstArg]), reinterpret_cast<uint32*>(&vmState.GPRegisters[secondArg]), reinterpret_cast<uint32*>(&vmState.GPRegisters[thirdArg]));
		break;
	}
	case vm::size::QWord:
	{
		flags = asmAND64(reinterpret_cast<uint64*>(&vmState.GPRegisters[firstArg]), reinterpret_cast<uint64*>(&vmState.GPRegisters[secondArg]), reinterpret_cast<uint64*>(&vmState.GPRegisters[thirdArg]));
		break;
	}
	default:
		throw ExceptionError("Size is incorrect.");
	}
	bool trapFlag = vmState.m_flags.getTrap();
	vmState.m_flags.setFlags(ConvertFlags(flags));
	vmState.m_flags.setTrap(trapFlag);
}

void vm::Processor::Executor::Or(State & vmState)
{
	int size = *((uchar*)&vmState.m_Command + 1);

	// first operand
	pRegister firstArg = getArgument(vmState.m_Command, argIndex::First);
	// second operand
	pRegister secondArg = getArgument(vmState.m_Command, argIndex::Second);
	// Get result operand
	pRegister thirdArg = getArgument(vmState.m_Command, argIndex::Third);

	int16 flags;
	switch (size)
	{
	case vm::size::Byte:
	{
		flags = asmOR8(reinterpret_cast<uint8*>(&vmState.GPRegisters[firstArg]), reinterpret_cast<uint8*>(&vmState.GPRegisters[secondArg]), reinterpret_cast<uint8*>(&vmState.GPRegisters[thirdArg]));
		break;
	}
	case vm::size::Word:
	{
		flags = asmOR16(reinterpret_cast<uint16*>(&vmState.GPRegisters[firstArg]), reinterpret_cast<uint16*>(&vmState.GPRegisters[secondArg]), reinterpret_cast<uint16*>(&vmState.GPRegisters[thirdArg]));
		break;
	}
	case vm::size::DWord:
	{
		flags = asmOR32(reinterpret_cast<uint32*>(&vmState.GPRegisters[firstArg]), reinterpret_cast<uint32*>(&vmState.GPRegisters[secondArg]), reinterpret_cast<uint32*>(&vmState.GPRegisters[thirdArg]));
		break;
	}
	case vm::size::QWord:
	{
		flags = asmOR64(reinterpret_cast<uint64*>(&vmState.GPRegisters[firstArg]), reinterpret_cast<uint64*>(&vmState.GPRegisters[secondArg]), reinterpret_cast<uint64*>(&vmState.GPRegisters[thirdArg]));
		break;
	}
	default:
		throw ExceptionError("Size is incorrect.");
	}
	bool trapFlag = vmState.m_flags.getTrap();
	vmState.m_flags.setFlags(ConvertFlags(flags));
	vmState.m_flags.setTrap(trapFlag);
}

void vm::Processor::Executor::Xor(State & vmState)
{
	int size = *((uchar*)&vmState.m_Command + 1);

	// first operand
	pRegister firstArg = getArgument(vmState.m_Command, argIndex::First);
	// second operand
	pRegister secondArg = getArgument(vmState.m_Command, argIndex::Second);
	// Get result operand
	pRegister thirdArg = getArgument(vmState.m_Command, argIndex::Third);

	int16 flags;
	switch (size)
	{
	case vm::size::Byte:
	{
		flags = asmXOR8(reinterpret_cast<uint8*>(&vmState.GPRegisters[firstArg]), reinterpret_cast<uint8*>(&vmState.GPRegisters[secondArg]), reinterpret_cast<uint8*>(&vmState.GPRegisters[thirdArg]));
		break;
	}
	case vm::size::Word:
	{
		flags = asmXOR16(reinterpret_cast<uint16*>(&vmState.GPRegisters[firstArg]), reinterpret_cast<uint16*>(&vmState.GPRegisters[secondArg]), reinterpret_cast<uint16*>(&vmState.GPRegisters[thirdArg]));
		break;
	}
	case vm::size::DWord:
	{
		flags = asmXOR32(reinterpret_cast<uint32*>(&vmState.GPRegisters[firstArg]), reinterpret_cast<uint32*>(&vmState.GPRegisters[secondArg]), reinterpret_cast<uint32*>(&vmState.GPRegisters[thirdArg]));
		break;
	}
	case vm::size::QWord:
	{
		flags = asmXOR64(reinterpret_cast<uint64*>(&vmState.GPRegisters[firstArg]), reinterpret_cast<uint64*>(&vmState.GPRegisters[secondArg]), reinterpret_cast<uint64*>(&vmState.GPRegisters[thirdArg]));
		break;
	}
	default:
		throw ExceptionError("Size is incorrect.");
	}
	bool trapFlag = vmState.m_flags.getTrap();
	vmState.m_flags.setFlags(ConvertFlags(flags));
	vmState.m_flags.setTrap(trapFlag);
}

void vm::Processor::Executor::Not(State & vmState)
{

	int size = *((uchar*)&vmState.m_Command + 1);

	// first operand
	pRegister firstArg = getArgument(vmState.m_Command, argIndex::First);
	uint16 flags;
	switch (size)
	{
	case vm::size::Byte:
	{
		flags = asmNOT8(reinterpret_cast<uint8*>(&vmState.GPRegisters[firstArg]));
		break;
	}
	case vm::size::Word:
	{
		flags = asmNOT16(reinterpret_cast<uint16*>(&vmState.GPRegisters[firstArg]));
		break;
	}
	case vm::size::DWord:
	{
		flags = asmNOT32(reinterpret_cast<uint32*>(&vmState.GPRegisters[firstArg]));
		break;
	}
	case vm::size::QWord:
	{
		flags = asmNOT64(reinterpret_cast<uint64*>(&vmState.GPRegisters[firstArg]));		
		break;
	}
	default:
		throw ExceptionError("Size is incorrect.");
	}
	bool trapFlag = vmState.m_flags.getTrap();
	vmState.m_flags.setFlags(ConvertFlags(flags));
	vmState.m_flags.setTrap(trapFlag);
}

void vm::Processor::Executor::Nand(State & vmState)
{
	int size = *((uchar*)&vmState.m_Command + 1);

	// first operand
	pRegister firstArg = getArgument(vmState.m_Command, argIndex::First);
	// second operand
	pRegister secondArg = getArgument(vmState.m_Command, argIndex::Second);
	// Get result operand
	pRegister thirdArg = getArgument(vmState.m_Command, argIndex::Third);

	int16 flags;
	switch (size)
	{
	case vm::size::Byte:
	{
		flags = asmNAND8(reinterpret_cast<uint8*>(&vmState.GPRegisters[firstArg]), reinterpret_cast<uint8*>(&vmState.GPRegisters[secondArg]), reinterpret_cast<uint8*>(&vmState.GPRegisters[thirdArg]));
		break;
	}
	case vm::size::Word:
	{
		flags = asmNAND16(reinterpret_cast<uint16*>(&vmState.GPRegisters[firstArg]), reinterpret_cast<uint16*>(&vmState.GPRegisters[secondArg]), reinterpret_cast<uint16*>(&vmState.GPRegisters[thirdArg]));
		break;
	}
	case vm::size::DWord:
	{
		flags = asmNAND32(reinterpret_cast<uint32*>(&vmState.GPRegisters[firstArg]), reinterpret_cast<uint32*>(&vmState.GPRegisters[secondArg]), reinterpret_cast<uint32*>(&vmState.GPRegisters[thirdArg]));
		break;
	}
	case vm::size::QWord:
	{
		flags = asmNAND64(reinterpret_cast<uint64*>(&vmState.GPRegisters[firstArg]), reinterpret_cast<uint64*>(&vmState.GPRegisters[secondArg]), reinterpret_cast<uint64*>(&vmState.GPRegisters[thirdArg]));
		break;
	}
	default:
		throw ExceptionError("Size is incorrect.");
	}
	bool trapFlag = vmState.m_flags.getTrap();
	vmState.m_flags.setFlags(ConvertFlags(flags));
	vmState.m_flags.setTrap(trapFlag);
}

void vm::Processor::Executor::Nor(State & vmState)
{
	int size = *((uchar*)&vmState.m_Command + 1);

	// first operand
	pRegister firstArg = getArgument(vmState.m_Command, argIndex::First);
	// second operand
	pRegister secondArg = getArgument(vmState.m_Command, argIndex::Second);
	// Get result operand
	pRegister thirdArg = getArgument(vmState.m_Command, argIndex::Third);

	int16 flags;
	switch (size)
	{
	case vm::size::Byte:
	{
		flags = asmNOR8(reinterpret_cast<uint8*>(&vmState.GPRegisters[firstArg]), reinterpret_cast<uint8*>(&vmState.GPRegisters[secondArg]), reinterpret_cast<uint8*>(&vmState.GPRegisters[thirdArg]));
		break;
	}
	case vm::size::Word:
	{
		flags = asmNOR16(reinterpret_cast<uint16*>(&vmState.GPRegisters[firstArg]), reinterpret_cast<uint16*>(&vmState.GPRegisters[secondArg]), reinterpret_cast<uint16*>(&vmState.GPRegisters[thirdArg]));
		break;
	}
	case vm::size::DWord:
	{
		flags = asmNOR32(reinterpret_cast<uint32*>(&vmState.GPRegisters[firstArg]), reinterpret_cast<uint32*>(&vmState.GPRegisters[secondArg]), reinterpret_cast<uint32*>(&vmState.GPRegisters[thirdArg]));
		break;
	}
	case vm::size::QWord:
	{
		flags = asmNOR64(reinterpret_cast<uint64*>(&vmState.GPRegisters[firstArg]), reinterpret_cast<uint64*>(&vmState.GPRegisters[secondArg]), reinterpret_cast<uint64*>(&vmState.GPRegisters[thirdArg]));
		break;
	}
	default:
		throw ExceptionError("Size is incorrect.");
	}
	bool trapFlag = vmState.m_flags.getTrap();
	vmState.m_flags.setFlags(ConvertFlags(flags));
	vmState.m_flags.setTrap(trapFlag);
}


void vm::Processor::Executor::Add(State& vmState)
{
	int size = *((uchar*)&vmState.m_Command + 1);

	// first operand
	pRegister firstArg = getArgument(vmState.m_Command, argIndex::First);
	// second operand
	pRegister secondArg = getArgument(vmState.m_Command, argIndex::Second);
	// Get result operand
	pRegister thirdArg = getArgument(vmState.m_Command, argIndex::Third);

	int16 flags;
	switch (size)
	{
	case vm::size::Byte :
	{
		flags = asmADD8(reinterpret_cast<uint8*>(&vmState.GPRegisters[firstArg]), reinterpret_cast<uint8*>(&vmState.GPRegisters[secondArg]), reinterpret_cast<uint8*>(&vmState.GPRegisters[thirdArg]));
		break;
	}
	case vm::size::Word :
	{
		flags = asmADD16(reinterpret_cast<uint16*>(&vmState.GPRegisters[firstArg]), reinterpret_cast<uint16*>(&vmState.GPRegisters[secondArg]), reinterpret_cast<uint16*>(&vmState.GPRegisters[thirdArg]));
		break;
	}
	case vm::size::DWord:
	{
		flags = asmADD32(reinterpret_cast<uint32*>(&vmState.GPRegisters[firstArg]), reinterpret_cast<uint32*>(&vmState.GPRegisters[secondArg]), reinterpret_cast<uint32*>(&vmState.GPRegisters[thirdArg]));
		break;
	}
	case vm::size::QWord:
	{
		flags = asmADD64(reinterpret_cast<uint64*>(&vmState.GPRegisters[firstArg]), reinterpret_cast<uint64*>(&vmState.GPRegisters[secondArg]), reinterpret_cast<uint64*>(&vmState.GPRegisters[thirdArg]));
		break;
	}
	default:
		throw ExceptionError("Size is incorrect.");
	}
	bool trapFlag = vmState.m_flags.getTrap();
	vmState.m_flags.setFlags(ConvertFlags(flags));
	vmState.m_flags.setTrap(trapFlag);
}

void vm::Processor::Executor::Sub(State & vmState)
{
	int size = *((uchar*)&vmState.m_Command + 1);

	// first operand
	pRegister firstArg = getArgument(vmState.m_Command, argIndex::First);
	// second operand
	pRegister secondArg = getArgument(vmState.m_Command, argIndex::Second);
	// Get result operand
	pRegister thirdArg = getArgument(vmState.m_Command, argIndex::Third);

	int16 flags;
	switch (size)
	{
	case vm::size::Byte:
	{
		flags = asmSUB8(reinterpret_cast<uint8*>(&vmState.GPRegisters[firstArg]), reinterpret_cast<uint8*>(&vmState.GPRegisters[secondArg]), reinterpret_cast<uint8*>(&vmState.GPRegisters[thirdArg]));
		break;
	}
	case vm::size::Word:
	{
		flags = asmSUB16(reinterpret_cast<uint16*>(&vmState.GPRegisters[firstArg]), reinterpret_cast<uint16*>(&vmState.GPRegisters[secondArg]), reinterpret_cast<uint16*>(&vmState.GPRegisters[thirdArg]));
		break;
	}
	case vm::size::DWord:
	{
		flags = asmSUB32(reinterpret_cast<uint32*>(&vmState.GPRegisters[firstArg]), reinterpret_cast<uint32*>(&vmState.GPRegisters[secondArg]), reinterpret_cast<uint32*>(&vmState.GPRegisters[thirdArg]));
		break;
	}
	case vm::size::QWord:
	{
		flags = asmSUB64(reinterpret_cast<uint64*>(&vmState.GPRegisters[firstArg]), reinterpret_cast<uint64*>(&vmState.GPRegisters[secondArg]), reinterpret_cast<uint64*>(&vmState.GPRegisters[thirdArg]));
		break;
	}
	default:
		throw ExceptionError("Size is incorrect.");
	}
	bool trapFlag = vmState.m_flags.getTrap();
	vmState.m_flags.setFlags(ConvertFlags(flags));
	vmState.m_flags.setTrap(trapFlag);
}

void vm::Processor::Executor::Mul(State & vmState)
{
	int size = *((uchar*)&vmState.m_Command + 1);

	// first operand
	pRegister firstArg = getArgument(vmState.m_Command, argIndex::First);
	// second operand
	pRegister secondArg = getArgument(vmState.m_Command, argIndex::Second);
	// Get result operand
	pRegister thirdArg = getArgument(vmState.m_Command, argIndex::Third);

	int16 flags;
	switch (size)
	{
	case vm::size::Byte:
	{
		flags = asmMUL8(reinterpret_cast<uint8*>(&vmState.GPRegisters[firstArg]), reinterpret_cast<uint8*>(&vmState.GPRegisters[secondArg]), reinterpret_cast<uint8*>(&vmState.GPRegisters[thirdArg]));
		break;
	}
	case vm::size::Word:
	{
		flags = asmMUL16(reinterpret_cast<uint16*>(&vmState.GPRegisters[firstArg]), reinterpret_cast<uint16*>(&vmState.GPRegisters[secondArg]), reinterpret_cast<uint16*>(&vmState.GPRegisters[thirdArg]));
		break;
	}
	case vm::size::DWord:
	{
		flags = asmMUL32(reinterpret_cast<uint32*>(&vmState.GPRegisters[firstArg]), reinterpret_cast<uint32*>(&vmState.GPRegisters[secondArg]), reinterpret_cast<uint32*>(&vmState.GPRegisters[thirdArg]));
		break;
	}
	case vm::size::QWord:
	{
		flags = asmMUL64(reinterpret_cast<uint64*>(&vmState.GPRegisters[firstArg]), reinterpret_cast<uint64*>(&vmState.GPRegisters[secondArg]), reinterpret_cast<uint64*>(&vmState.GPRegisters[thirdArg]));
		break;
	}
	default:
		throw ExceptionError("Size is incorrect.");
	}
	bool trapFlag = vmState.m_flags.getTrap();
	vmState.m_flags.setFlags(ConvertFlags(flags));
	vmState.m_flags.setTrap(trapFlag);
}

void vm::Processor::Executor::Div(State & vmState)
{
	int size = *((uchar*)&vmState.m_Command + 1);

	// first operand
	pRegister firstArg = getArgument(vmState.m_Command, argIndex::First);
	// second operand
	pRegister secondArg = getArgument(vmState.m_Command, argIndex::Second);
	// Get result operand
	pRegister thirdArg = getArgument(vmState.m_Command, argIndex::Third);

	int16 flags;
	switch (size)
	{
	case vm::size::Byte:
	{
		flags = asmDIV8(reinterpret_cast<uint8*>(&vmState.GPRegisters[firstArg]), reinterpret_cast<uint8*>(&vmState.GPRegisters[secondArg]), reinterpret_cast<uint8*>(&vmState.GPRegisters[thirdArg]));
		break;
	}
	case vm::size::Word:
	{
		flags = asmDIV16(reinterpret_cast<uint16*>(&vmState.GPRegisters[firstArg]), reinterpret_cast<uint16*>(&vmState.GPRegisters[secondArg]), reinterpret_cast<uint16*>(&vmState.GPRegisters[thirdArg]));
		break;
	}
	case vm::size::DWord:
	{
		flags = asmDIV32(reinterpret_cast<uint32*>(&vmState.GPRegisters[firstArg]), reinterpret_cast<uint32*>(&vmState.GPRegisters[secondArg]), reinterpret_cast<uint32*>(&vmState.GPRegisters[thirdArg]));
		break;
	}
	case vm::size::QWord:
	{
		flags = asmDIV64(reinterpret_cast<uint64*>(&vmState.GPRegisters[firstArg]), reinterpret_cast<uint64*>(&vmState.GPRegisters[secondArg]), reinterpret_cast<uint64*>(&vmState.GPRegisters[thirdArg]));
		break;
	}
	default:
		throw ExceptionError("Size is incorrect.");
	}
	bool trapFlag = vmState.m_flags.getTrap();
	vmState.m_flags.setFlags(ConvertFlags(flags));
	vmState.m_flags.setTrap(trapFlag);
}

void vm::Processor::Executor::Inc(State & vmState)
{
	int size = *((uchar*)&vmState.m_Command + 1);

	// first operand
	pRegister firstArg = getArgument(vmState.m_Command, argIndex::First);
	uint16 flags;
	switch (size)
	{
	case vm::size::Byte:
	{
		flags = asmINC8(reinterpret_cast<uint8*>(&vmState.GPRegisters[firstArg]));
		break;
	}
	case vm::size::Word:
	{
		flags = asmINC16(reinterpret_cast<uint16*>(&vmState.GPRegisters[firstArg]));
		break;
	}
	case vm::size::DWord:
	{
		flags = asmINC32(reinterpret_cast<uint32*>(&vmState.GPRegisters[firstArg]));
		break;
	}
	case vm::size::QWord:
	{
		flags = asmINC64(reinterpret_cast<uint64*>(&vmState.GPRegisters[firstArg]));
		break;
	}
	default:
		throw ExceptionError("Size is incorrect.");
	}
	bool trapFlag = vmState.m_flags.getTrap();
	vmState.m_flags.setFlags(ConvertFlags(flags));
	vmState.m_flags.setTrap(trapFlag);
}

void vm::Processor::Executor::Dec(State & vmState)
{

	int size = *((uchar*)&vmState.m_Command + 1);

	// first operand
	pRegister firstArg = getArgument(vmState.m_Command, argIndex::First);
	uint16 flags;
	switch (size)
	{
	case vm::size::Byte:
	{
		flags = asmDEC8(reinterpret_cast<uint8*>(&vmState.GPRegisters[firstArg]));
		break;
	}
	case vm::size::Word:
	{
		flags = asmDEC16(reinterpret_cast<uint16*>(&vmState.GPRegisters[firstArg]));
		break;
	}
	case vm::size::DWord:
	{
		flags = asmDEC32(reinterpret_cast<uint32*>(&vmState.GPRegisters[firstArg]));
		break;
	}
	case vm::size::QWord:
	{
		flags = asmDEC64(reinterpret_cast<uint64*>(&vmState.GPRegisters[firstArg]));
		break;
	}
	default:
		throw ExceptionError("Size is incorrect.");
	}
	bool trapFlag = vmState.m_flags.getTrap();
	vmState.m_flags.setFlags(ConvertFlags(flags));
	vmState.m_flags.setTrap(trapFlag);
}

void vm::Processor::Executor::Push(State & vmState)
{
	int size = *((uchar*)&vmState.m_Command + 1);

	// first operand
	pRegister firstArg = getArgument(vmState.m_Command, argIndex::First);
	if (firstArg == SF) {
		firstArg = vmState.addressRegisters[SF - addresRegZero];
	}
	switch (size) {
	case Byte:
		vmState.m_SP -= sizeof(uint8);
		if (vmState.m_SP < vmState.m_StackLowerBound)
			throw ExceptionError("Runtime error in PUSH: Stack overflow.");
		m_Memory->At<uint8>(vmState.m_SP) = *reinterpret_cast<uint8*>(&firstArg);
		break;
	case Word:
		vmState.m_SP -= sizeof(uint16);
		if (vmState.m_SP < vmState.m_StackLowerBound)
			throw ExceptionError("Runtime error in PUSH: Stack overflow.");
		m_Memory->At<uint16>(vmState.m_SP) = *reinterpret_cast<uint16*>(&firstArg);
		break;
	case DWord:
		vmState.m_SP -= sizeof(uint32);
		if (vmState.m_SP < vmState.m_StackLowerBound)
			throw ExceptionError("Runtime error in PUSH: Stack overflow.");
		m_Memory->At<uint32>(vmState.m_SP) = *reinterpret_cast<uint32*>(&firstArg);
		break;
	case QWord:
		vmState.m_SP -= sizeof(uint64);
		if (vmState.m_SP < vmState.m_StackLowerBound)
			throw ExceptionError("Runtime error in PUSH: Stack overflow.");
		m_Memory->At<uint64>(vmState.m_SP) = *reinterpret_cast<uint64*>(&firstArg);
		break;
	default:
		throw ExceptionError("Runtime error: Unknown operand size.");
}
}

void vm::Processor::Executor::Pop(State & vmState)
{

	int size = *((uchar*)&vmState.m_Command + 1);

	// first operand
	pRegister firstArg = getArgument(vmState.m_Command, argIndex::First);
	if (firstArg == SF) {
		firstArg = vmState.addressRegisters[SF - addresRegZero];
	}
	switch (size) {
	case Byte:
		if ((vmState.m_SP + int(sizeof(uint8))) > vmState.m_StackUpperBound)
			throw ExceptionError("Runtime error in POP: Stack underflow.");
		*reinterpret_cast<uint8*>(&firstArg) = m_Memory->At<uint8>(vmState.m_SP);
		vmState.m_SP += sizeof(uint8);
		break;
	case Word:
		if ((vmState.m_SP + int(sizeof(uint16))) > vmState.m_StackUpperBound)
			throw ExceptionError("Runtime error in POP: Stack underflow.");
		*reinterpret_cast<uint16*>(&firstArg) = m_Memory->At<uint16>(vmState.m_SP);
		vmState.m_SP += sizeof(uint16);
		break;
	case DWord:
		if ((vmState.m_SP + int(sizeof(uint32))) > vmState.m_StackUpperBound)
			throw ExceptionError("Runtime error in POP: Stack underflow.");
		*reinterpret_cast<uint32*>(&firstArg) = m_Memory->At<uint32>(vmState.m_SP);
		vmState.m_SP += sizeof(uint32);
		break;
	case QWord:
		if ((vmState.m_SP + int(sizeof(uint64))) > vmState.m_StackUpperBound)
			throw ExceptionError("Runtime error in POP: Stack underflow.");
		*reinterpret_cast<uint64*>(&firstArg) = m_Memory->At<uint64>(vmState.m_SP);
		vmState.m_SP += sizeof(uint64);
		break;
	default:
		throw ExceptionError("Runtime error: Unknown operand size.");
	}
}

void vm::Processor::Executor::PushF(State& vmState)
{
	vmState.m_SP -= sizeof(uint16);
	if (vmState.m_SP < vmState.m_StackLowerBound)
		throw ExceptionError("Runtime error in PUSHF: Stack overflow.");
	m_Memory->At<uint16>(vmState.m_SP) = vmState.m_flags.getFlags();
}

void vm::Processor::Executor::PopF(State& vmState)
{

	if ((vmState.m_SP + int(sizeof(uint16))) > vmState.m_StackUpperBound)
		throw ExceptionError("Runtime error in POP: Stack underflow.");
	uint16 uint16Flags = m_Memory->At<uint16>(vmState.m_SP);
	vmState.m_SP += sizeof(uint16);
	vmState.m_flags.setFlags(uint16Flags);
}

void vm::Processor::Executor::Load(State & vmState)
{
	int size = *((uchar*)&vmState.m_Command + 1);

	// first operand
	pRegister valueReg = getArgument(vmState.m_Command, argIndex::First);
	// second operand
	pRegister addressRegister = getArgument(vmState.m_Command, argIndex::Second);
	uint8 Address = *reinterpret_cast<uint8*>(&vmState.addressRegisters[addressRegister - addresRegZero]);

	switch (size)
	{
	case Byte:
		if ((Address + sizeof(uint8)) > m_Memory->GetSize())
			throw ExceptionError("Runtime error in LOAD: Out of memory bounds read.");
		vmState.GPRegisters[valueReg] = m_Memory->At<uint8>(Address);
		break;
	case Word:
		if ((Address + sizeof(uint16)) > m_Memory->GetSize())
			throw ExceptionError("Runtime error in LOAD: Out of memory bounds read.");
		vmState.GPRegisters[valueReg] = m_Memory->At<uint16>(Address);
		break;
	case DWord:
		if ((Address + sizeof(uint32)) > m_Memory->GetSize())
			throw ExceptionError("Runtime error in LOAD: Out of memory bounds read.");
		vmState.GPRegisters[valueReg] = m_Memory->At<uint32>(Address);
		break;
	case QWord:
		if ((Address + sizeof(uint64)) > m_Memory->GetSize())
			throw ExceptionError("Runtime error in LOAD: Out of memory bounds read.");
		vmState.GPRegisters[valueReg] = m_Memory->At<uint64>(Address);
		break;
	default:
		throw ExceptionError("Runtime error: Unknown operand size.");
	}
}

void vm::Processor::Executor::Store(State & vmState)
{
	// first operand
	pRegister valueRegister = getArgument(vmState.m_Command, argIndex::First);
	// second operand
	pRegister addressRegister = getArgument(vmState.m_Command, argIndex::Second);
	int size = *((uchar*)&vmState.m_Command + 1);
	uint8 Address = *reinterpret_cast<uint8*>(&vmState.addressRegisters[addressRegister - addresRegZero]);

	switch (size)
	{
	case Byte:
		if ((Address + sizeof(uint8)) > m_Memory->GetSize())
			throw ExceptionError("Runtime error in STORE: Out of memory bounds write.");
		m_Memory->At<uint8>(Address) = *reinterpret_cast<uint8*>(&vmState.GPRegisters[valueRegister]);
		break;
	case Word:
		if ((Address + sizeof(uint16)) > m_Memory->GetSize())
			throw ExceptionError("Runtime error in STORE: Out of memory bounds write.");
		m_Memory->At<uint16>(Address) = *reinterpret_cast<uint16*>(&vmState.GPRegisters[valueRegister]);
		break;
	case DWord:
		if ((Address + sizeof(uint32)) > m_Memory->GetSize())
			throw ExceptionError("Runtime error in STORE: Out of memory bounds write.");
		m_Memory->At<uint32>(Address) = *reinterpret_cast<uint32*>(&vmState.GPRegisters[valueRegister]);
		break;
	case QWord:
		if ((Address + sizeof(uint64)) > m_Memory->GetSize())
			throw ExceptionError("Runtime error in STORE: Out of memory bounds write.");
		m_Memory->At<uint64>(Address) = *reinterpret_cast<uint64*>(&vmState.GPRegisters[valueRegister]);
		break;
	default:
		throw ExceptionError("Runtime error: Unknown operand size.");
	}
}

uint16 vm::Processor::Executor::ConvertFlags(uint16 Flags)
{
	Processor::Flags flags;
	flags.setCarry(0 != (Flags & 0x0001));
	flags.setParity(0 != (Flags & 0x0004));
	flags.setZero(0 != (Flags & 0x0040));
	flags.setSign(0 != (Flags & 0x0080));
	flags.setOverflow(0 != (Flags & 0x0800));
	return flags.getFlags();
}
