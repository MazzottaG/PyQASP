#include <iostream>
#include "antlr4-runtime.h"
#include "parser/ASPCore2Lexer.h"
#include "parser/ASPCore2Parser.h"
#include "parser/PredicateListener.h"

int main(int argc, char *argv[])
{
	std::string toParse(argv[1]);

	antlr4::ANTLRInputStream input;
	input.load(toParse);
	// std::call_once(printFlag,print);
	ASPCore2Lexer lexer (&input);
	antlr4::CommonTokenStream tokens(&lexer);
	ASPCore2Parser parser (&tokens);
	PredicateListener c;
	parser.addParseListener(&c);
	parser.program();
	c.printPredicates();
	
}