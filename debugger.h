#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "Interpretator.h"

// STL
#include <unordered_map>

namespace vm {

	class Debugger : public IDebugger
	{
	public:
		Debugger(Interpretator* Machine);

	public:
		bool Start();
		void End(bool bSuccessfull);
		bool Break();
		void PrintNextCommand();
		void checkBreak();
	private:
		// Internal routines
		void PrintState(Processor::State const& tState);
	private:
		Interpretator*		m_Machine;
		std::string const	m_BreakCmd;
		int m_breakPtrLine;
		std::vector<int> m_breakPtrs;
		//std::unordered_map<size_t, std::string>	m_mapCodeCache;
		char				m_debugCommand;
	};
}
#endif
