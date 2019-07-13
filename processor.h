#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "base_definitions.h"
//#include "command.h"
#include "memory.h"


namespace vm {
	//
	//	Interface for Debuggers
	//
	class IDebugger
	{
	public:
		virtual bool Start() = 0;
		virtual void End(bool bSuccessfull) = 0;
		virtual bool Break() = 0;
		//helper
		virtual void PrintNextCommand() = 0;
		virtual void checkBreak() = 0;
	};

	//Processor class or CPU contains all main logic: execution of commands
	class Processor {
	public:
		Processor(Memory& memory, size_t StackSize, int pc);
		int run();
		void AttachDebugger(IDebugger* Debugger);
		void DetachDebugger();
		inline void SetPC(size_t NewPC);
	public:
		class Flags {
		public:
			Flags();
			bool IsEqual() const;
			bool IsNotEqual() const;
			bool IsAbove() const;
			bool IsAboveOrEqual() const;
			bool IsBelow() const;
			bool IsBelowOrEqual() const;
			bool IsGreat() const;
			bool IsGreatOrEqual() const;
			bool IsLow() const;
			bool IsLowOrEqual() const;
			bool IsOverflow() const;
			bool IsNotOverflow() const;
			bool IsSigned() const;
			bool IsNotSigned() const;
			bool IsParity() const;
			bool IsNotParity() const;

			inline void setTrap(bool value);
			inline void setCarry(bool value);
			inline void setParity(bool value);
			inline void setZero(bool value);
			inline void setSign(bool value);
			inline void setOverflow(bool value);
			inline void setFlags(uint values);  //set all flags

			inline bool getTrap() const;
			inline bool getCarry() const;
			inline bool getParity() const;
			inline bool getZero() const;
			inline bool getSign() const;
			inline bool getOverflow() const;
			inline uint getFlags() const;     //get all flags
		private:
			enum masks {
				emptyFlags = 0x0000,
				CF = 0x0001,
				PF = 0x0002,
				ZF = 0x0004,
				SF = 0x0008,
				OF = 0x0010,
				TF = 0x0080,
			};
			int16 m_flagsRegister;
		};
	public:
		enum
		{
			AddressRegistersSize = 8,
			GPRegistersSize = 32
		};

		struct State {           //this struct created for making state switching easy
			pRegister m_PC;      // program counter
			pRegister m_Command; // executable code Registor
			uint m_CommandSize;  // size of command is not constant
			Flags m_flags;       // flag register
			bool m_ContinueRun;
			uint32 addressRegisters[AddressRegistersSize]; // address registers
			uint GPRegisters[GPRegistersSize];            // general purpose registers

														  //Stack
			offset m_SP;
			offset m_SF;
			offset m_StackLowerBound;
			offset m_StackUpperBound;
		public:
			State();
			State & operator=(State const& state2);
		};


		class Executor {
		public:
			Executor(MemoryPtr pMemory);

			// commands without arguments
			void Nop(State& vmState);
			void Exit(State& vmState);
			void Break(State& bmState);

			// Execution control
			void JumpOffset(State& vmState);
			void JumpAddress(State& vmState);
			void Call(State& vmState);
			void Ret(State& vmState) ;

			// Assignment
			void AssignNumber(State& vmState);
			void AssignAdress(State& vmState);
			void Move(State& vmState);
			void Swap(State& vmState);

			// Logical
			void And(State& vmState);
			void Or(State& vmState);
			void Xor(State& vmState);
			void Not(State& vmState);
			void Nand(State& vmState);
			void Nor(State& vmState);

			// Unsigned integral arithmetic
			void Add(State& vmState);
			void Sub(State& vmState);
			void Mul(State& vmState);
			void Div(State& vmState);
			void Inc(State& vmState);
			void Dec(State& vmState);

			// work with Stack
			void Push(State& vmState);
			void Pop(State& vmState);
			void PushF(State& vmState);
			void PopF(State& vmState);

			//work with Memory
			void Load(State& vmState);
			void Store(State& vmState);
		private:
			Memory* m_Memory;
			// Converts IA32 Flags into own flags
			static uint16 ConvertFlags(uint16 Flags);
		public:
			enum class argIndex
			{
				First,
				Second,
				Third
			};
			static pRegister getArgument(pRegister command, argIndex argumentIndex);
		};

		typedef Executor* ExecutorPtr;
		typedef void(Executor::*exec)(vm::Processor::State&);
	private:
		pRegister fetch();
		exec decode();

	private:
		ExecutorPtr  m_Executor;
		State        m_State;           // processor state
		Memory       m_Memory;     // pointer to memory(RAM)
		IDebugger* m_Debugger;

	public:
		inline State& GetState();
		void SetState(State const& newState);
		inline Flags& GetFlags();
	};

	typedef Processor* ProcessorPtr;

	/*Processor::Flags::Flags()
		:m_flagsRegister(0)
	{
	}*/

	//----------------------------------------------
	// get processor state
	//----------------------------------------------
	inline Processor::State& Processor::GetState()
	{
		return m_State;
	}

	inline void Processor::SetPC(size_t newPC)
	{
		m_State.m_PC = newPC;
	}

	inline Processor::Flags& Processor::GetFlags()
	{
		return m_State.m_flags;
	}
	//-----------------------------------------------
	// Processor::Flags set flags functions
	//------------------------------------------------
	inline void Processor::Flags::setTrap(bool value)
	{
		m_flagsRegister = value ? (m_flagsRegister | TF) : (m_flagsRegister & ~TF);
	}

	inline void Processor::Flags::setCarry(bool value)
	{
		m_flagsRegister = value ? (m_flagsRegister | CF) : (m_flagsRegister & ~CF);
	}

	inline void Processor::Flags::setParity(bool value)
	{
		m_flagsRegister = value ? (m_flagsRegister | PF) : (m_flagsRegister & ~PF);
	}

	inline void Processor::Flags::setZero(bool value)
	{
		m_flagsRegister = value ? (m_flagsRegister | ZF) : (m_flagsRegister & ~ZF);
	}

	inline void Processor::Flags::setSign(bool value)
	{
		m_flagsRegister = value ? (m_flagsRegister | SF) : (m_flagsRegister & ~SF);
	}

	inline void Processor::Flags::setOverflow(bool value)
	{
		m_flagsRegister = value ? (m_flagsRegister | OF) : (m_flagsRegister & ~OF);
	}

	inline void Processor::Flags::setFlags(uint values)
	{
		m_flagsRegister = (m_flagsRegister & emptyFlags) | values;
	}

	//-----------------------------------------------------
	// Processor::Flags get flags functions
	//-----------------------------------------------------
	inline bool Processor::Flags::getTrap() const
	{
		return m_flagsRegister & TF;
	}

	inline bool Processor::Flags::getCarry() const
	{
		return m_flagsRegister & CF;
	}

	inline bool Processor::Flags::getParity() const
	{
		return m_flagsRegister & PF;
	}

	inline bool Processor::Flags::getZero() const
	{
		return m_flagsRegister & ZF;
	}

	inline bool Processor::Flags::getSign() const
	{
		return m_flagsRegister & SF;
	}

	inline bool Processor::Flags::getOverflow() const
	{
		return m_flagsRegister & OF;
	}

	inline uint Processor::Flags::getFlags() const
	{
		return m_flagsRegister;
	}
}
#endif
