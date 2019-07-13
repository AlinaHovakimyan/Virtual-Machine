#include "interpretator.h"
#include "Preprocessor.h"
#include "debugger.h"

int main() {
	try {
		std::string obj_file = "";
		std::string format = "bin";
		//std::cout << "file is binary or assembly(enter bin or asm):" << std::endl;
		//std::cin >> format;
		if (format == "bin") {
			std::cout << "Please enter file name, that you want to execute" << std::endl;
			std::cin >> obj_file;
			std::ifstream objInput(obj_file, std::ios::in);
			if (!objInput) {
				throw ExceptionError(obj_file + "can't be opened\n");
			}

			vm::InterpretatorPtr vMachine = new vm::Interpretator();
			vm::IDebugger* Debugger = new vm::Debugger(vMachine);
			vMachine->init(objInput);
			vMachine->SetDebugger(Debugger);
			vMachine->run();
		}
		else if (format == "asm") {
			//std::cout << "Please enter file name, that you want to execute" << std::endl;
			std::cin >> obj_file;
			std::ifstream objInput(obj_file, std::ios::in);
			PreProcessor preprocessor(obj_file);
			preprocessor.process();
			vm::InterpretatorPtr vMachine = new vm::Interpretator();
			vMachine->init(objInput);
			vMachine->run();
		}
		else {
			throw ExceptionError("Invalid format");
		}
	}
	catch (ExceptionError exception) {
		std::cout << "Error: " << exception.message() << std::endl;
	}
	return 0;
}