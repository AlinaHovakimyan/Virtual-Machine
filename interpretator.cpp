#include "Interpretator.h"

void vm::Interpretator::init(std::ifstream& inputS)
{
	// run the loader 
	int pc = Loader::run(inputS, m_memory);
	m_memory.Resize(m_memory.GetSize() + StackDefSize);

	// Create the CPU and intialize
	m_CPU = new Processor(m_memory, StackDefSize, pc);

	if (m_Debugger != nullptr) {
		m_CPU->AttachDebugger(m_Debugger);
	}
}

int vm::Interpretator::run()
{
	m_CPU->run();
	return 0;           // returns 0 if everything is ok
}

void vm::Interpretator::SetDebugger(vm::IDebugger* Debugger)
{
	m_Debugger = Debugger;
	if (m_CPU != nullptr) {
		if (m_Debugger != nullptr) {
			m_CPU->AttachDebugger(m_Debugger);
		} else {
			m_CPU->DetachDebugger();
		}
	}
}