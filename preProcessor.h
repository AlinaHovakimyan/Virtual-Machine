#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H
#include <map>
#include <string>
#include "command.h"
#include <fstream>

class PreProcessor {
public:
	PreProcessor(std::string assemblyFile);
	~PreProcessor();
	void process();
private:
	void tokenize();
	void parse();
private:
	std::ifstream m_assemblyFile;
};

#endif
