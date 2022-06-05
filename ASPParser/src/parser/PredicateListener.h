#ifndef CONCRETE_LISTENER_H
#define CONCRETE_LISTENER_H
#include "ASPCore2BaseListener.h"
#include <vector>
#include <string>
#include <iostream>
class PredicateListener : public ASPCore2BaseListener{
    private:
        bool readingAtom;
        bool readingAtomIdentifier;
        bool errorAppened;
        std::vector<std::string> predicates;
    public:
        PredicateListener(){
            errorAppened=false;
            readingAtom=false;
            readingAtomIdentifier=false;
        }
        void printPredicates()const{
            for(const std::string& s : predicates){
                std::cout << s << std::endl;
            }
        }
        void enterAtom(ASPCore2Parser::AtomContext * /*ctx*/) override { readingAtom=true;}
        void exitAtom(ASPCore2Parser::AtomContext * /*ctx*/) override {}

        virtual void enterIdentifier(ASPCore2Parser::IdentifierContext * /*ctx*/) override { if(readingAtom) readingAtomIdentifier=true;}
        virtual void exitIdentifier(ASPCore2Parser::IdentifierContext * /*ctx*/) override { if(readingAtom){readingAtomIdentifier=false;readingAtom=false;}}

        virtual void visitTerminal(antlr4::tree::TerminalNode * t) override {if(readingAtomIdentifier)predicates.push_back(t->getText());}
        virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override {
            errorAppened=true;
            std::cout << "Unable to parse program"<<std::endl;
            exit(180);
        }
};
#endif