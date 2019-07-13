#ifndef LOADER_H
#define LOADER_H

#include "memory.h"
#include <fstream>

namespace vm {
	//----------------------------------------------------
	// Loader loads code from binary file to virtual machines memory
	//----------------------------------------------------
	class Loader {
	public:
		static int run(std::istream& oInput,
			           Memory& memory);
	private:
		//functions to load from file to m_Memory;
	private:
		std::ifstream& m_objFile;
	};

	typedef Loader* LoaderPtr;
}

#endif
