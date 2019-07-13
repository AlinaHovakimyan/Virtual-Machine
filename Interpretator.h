#ifndef INTERPRETATOR_H
#define INTERPRETATOR_H

#include "declarations.h"
#include "processor.h"
#include "Loader.h"

// namespace virtual machine

namespace vm {

	//----------------------------------------------------------
	// Interpretator operates Loader for loading  code to memory
	// and processor for executing the code
	//-----------------------------------------------------------
	class Interpretator {
	public:
		// Initialise interpretator with stream which contains executable program
		void init(std::ifstream& inputS);

		int run();


		// Attach/detach debugger
		void SetDebugger(IDebugger* debugger);

		//inline CodePtr GetCode() const;
		inline ProcessorPtr GetProcessor() const;
	private:
		Memory m_memory;         // maybe program will need MemoryMeneger, but i havn't created that class yet
		ProcessorPtr m_CPU;
		IDebugger* m_Debugger;
	};

	typedef Interpretator* InterpretatorPtr;

}


inline vm::Processor* vm::Interpretator::GetProcessor() const
{
	return m_CPU;
}

#endif
