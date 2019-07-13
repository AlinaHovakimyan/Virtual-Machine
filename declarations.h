
#ifndef DECLARATIONS_H
#define DECLARATIONS_H

#define wordSize 4

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <deque>
#include <stack>
#include <vector>
#include <list>
#include <map>
#include <set>


enum tokenType {
	identifire,
	number,
	comma,
	section_indentifire
};

enum class tokenizerState {
	initial_state,
	identifire_state,
	point_state,
	section_state,
	digit_state,
	comma_state
};

enum class parserStates {
	Initial_state,
	var_state,
	func_state,
	assign_state,
	operations_state,
	num_state,
	op_bracket_state,
	cl_bracket_state,
	semicolon_state,
	comma_state
};

const std::set<std::string> keywords = {
	"ADD",
	"SUB",
	"MUL",
	"DIV",
	"LOAD",
	"MOVE",
	"STORE",
	"CALL",
	"EXIT",
	"RET",
	"ASSIGN",
	"R0",
	"R1",
	"R2",
	"R3",
	"R4",
	"R5",
	"R6",
	"R7",
	"A0",
	"A1",
	"A2"
};


class ExceptionError {
	std::string exceptionMessage;
public:
	ExceptionError(std::string message) : exceptionMessage(message) {};
	std::string message() { return exceptionMessage; };
};

struct TokenInf {
	std::string nameOrValue;
	tokenType e_tokType;
	TokenInf(std::string name = "", tokenType type = tokenType()) :
		nameOrValue(name), e_tokType(type)
	{}
	virtual ~TokenInf() {}
};

typedef TokenInf* TokenInfPtr;

struct Node {
	Node();
	Node(TokenInfPtr);
	virtual ~Node() {};
	Node* left;
	Node* right;
	Node* parent;
	TokenInfPtr value;
};


struct func_map_inf {
	bool b_isFunc;
	size_t argCount;
	std::vector<std::string> args;
};
#endif
