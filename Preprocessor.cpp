#include "Preprocessor.h"
#include "declarations.h"
#include "command.h"
#include <sstream>

//must be lexer parser

PreProcessor::PreProcessor(std::string assemblyFile)
	:m_assemblyFile(assemblyFile, std::ios::in)
{
	if (!m_assemblyFile) {
		throw ExceptionError(assemblyFile + "canot be opened\n");
    }
}

PreProcessor::~PreProcessor()
{
	m_assemblyFile.close();
}

void PreProcessor::process()
{
	tokenize();
}

void PreProcessor::tokenize()
{
	std::string line = "";
	std::vector<std::string> words;
	std::vector<int64_t> commands;

	while (!m_assemblyFile.eof()) {
		std::getline(m_assemblyFile, line);
		
		// split line of command word by word
		std::istringstream buf(line);
		for (std::string word; buf >> word; ) {
			words.push_back(word);
	    }
		
		if (vm::CommandParser::CommandOpCodes.find(words[0]) == vm::CommandParser::CommandOpCodes.end()) {
			throw ExceptionError("Invalid command name");
		}
		commands.push_back(vm::CommandParser::CommandOpCodes.find(words[0])->second);
		
		if (words[1] == "B") {
			commands.push_back(vm::size::Byte);
		}
		else if (words[1] == "W") {
			commands.push_back(vm::size::Word);
		}
		else if (words[1] == "DW") {
			commands.push_back(vm::size::DWord);
		}
		else if (words[1] == "QW") {
			commands.push_back(vm::size::QWord);
		}
		else {
			throw ExceptionError("Invalide size type specifier");
		}

		// parse all arguments 
		//write commands to memory

		words.clear();
	}
}

void PreProcessor::parse()
{
}
