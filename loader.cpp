#include <fstream>
#include <iterator>
#include "loader.h"
#include "declarations.h"

namespace vm {
	std::unordered_map<int, std::string> varFuncMap;
}

int vm::Loader::run(std::istream& oInput, Memory& memory)
{
	std::vector<uchar> vec;
	vec.push_back(0);
	int count = 1;
	char symbol = 0;
	int pc = 0;
	 // copies all symbols into symbol table buffer
	oInput.seekg(0, std::ios_base::beg);    // symbol table ptr is writen in header file in first 4 bytes
	
	oInput.read(reinterpret_cast<char*>(&pc), sizeof(int));
	oInput.read(reinterpret_cast<char*>(&pc), sizeof(int));
	
	int varCount = 0;
	int offset = 0;
	int nameSize = 0;
	std::string name = "";
	oInput.read(reinterpret_cast<char*>(&varCount), sizeof(int));

	while (varCount) {
		oInput.read(reinterpret_cast<char*>(&offset), sizeof(int));
		oInput.read(reinterpret_cast<char*>(&nameSize), sizeof(int));

		char* temp = new char[nameSize + 1];
		oInput.read(temp, nameSize);
		temp[nameSize] = '\0';
		name = temp;
		delete[] temp;
		varFuncMap.insert({offset, name});
		--varCount;
	}
	int i = 0;
	while (i < pc - 1) {
		vec.push_back(0);
		++i;
	}
	while (!oInput.eof()) {
		oInput.read(reinterpret_cast<char*>(&symbol), sizeof(char));
		vec.push_back(symbol);
	}
	memory = vec;
	return pc;
}
