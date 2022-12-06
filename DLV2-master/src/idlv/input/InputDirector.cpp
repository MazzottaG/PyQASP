/*******************************************************************************
 * Copyright 2016 Francesco Calimeri, Davide Fusca', Simona Perri and Jessica Zangari
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************/

#include <iostream>
#include <string>
#include <cstring>
#include "InputDirector.h"
#include "../../common/Assert.h"
#include "../../common/ErrorMessage.h"
#include "aspcore2_lexer.hpp"
#include "aspcore2_parser.hpp"

using namespace std;
using namespace DLV2;

//////////////////////////////////////////////////////////////////////////////
// Some LEX/FLEX dependencies

extern "C" int yywrap() { return 1; }    // End-of-file handler for LEX
extern FILE* yyin;     // Where LEX reads its input from

/*
 * local variable for parsing file
 */
static ParsingVar pv;
/*--------------------------------------------------------------------
 * getNextLine
 *
 * reads a line into the buffer
 *------------------------------------------------------------------*/
static
int getNextLine(void) {
  char *p;

  /*================================================================*/
  /*----------------------------------------------------------------*/
  pv.nBuffer = 0;
  pv.nTokenStart = -1;
  pv.nTokenNextStart = 1;
  pv.eof = false;

  /*================================================================*/
  /* read a line ---------------------------------------------------*/
  p = fgets(pv.buffer, pv.lMaxBuffer, yyin);
  if (  p == NULL  ) {
    if (  ferror(yyin)  )
      return -1;
    pv.eof = true;
    return 1;
  }

  pv.nRow += 1;
  pv.lBuffer = strlen(pv.buffer);

  /*================================================================*/
  /* that's it -----------------------------------------------------*/
  return 0;
}
/*--------------------------------------------------------------------
 * GetNextChar
 *
 * reads a character from input for flex
 *------------------------------------------------------------------*/
extern
int GetNextChar(char *b, int maxBuffer) {
  int frc;

  /*================================================================*/
  /*----------------------------------------------------------------*/
  if (  pv.eof  )
    return 0;

  /*================================================================*/
  /* read next line if at the end of the current -------------------*/
  while (  pv.nBuffer >= pv.lBuffer  ) {
    frc = getNextLine();
    if (  frc != 0  )
      return 0;
    }

  /*================================================================*/
  /* ok, return character ------------------------------------------*/
  b[0] = pv.buffer[pv.nBuffer];
  pv.nBuffer += 1;

  return b[0]==0?0:1;
}

int 
yyerror( 
    InputDirector& director,
    const char* msg ) 
{ 


    return director.onError(msg); 
}
    
void 
InputDirector::configureBuilder( 
    InputBuilder* b )
{
    assert_msg( b != NULL, "The input builder is null." );
    builder = b;  	
}

// Set a new ParserConstraint after deleting the current one. 
// The director acquires the ownership of the input ParserConstraint object, 
// and, as owner, it is deleting its ParserConstraint in the destructor.
void 
InputDirector::setParserConstraint( 
    ParserConstraint* c )
{
    assert_msg( c != NULL, "The parser controller is null." );
    if( parserConstraint )
        delete parserConstraint;
    parserConstraint = c;
}

InputDirector::InputDirector():   
    parserLine(0),
    parserFile(NULL),
    parserErrors(0),
    parserStateInternal(false),
    builder(NULL),
    parserConstraint(new ParserConstraint()),
	findError(false)
{
    
}

InputDirector::~InputDirector()
{
    if( parserConstraint )
        delete parserConstraint;
}

int
InputDirector::parse(
    vector<const char*> files ) 
{
    if( files.size() == 0 )
        ErrorMessage::errorGeneric( "No input files." );
//    else if( files.size() == 1 && !strcmp(files[0],"--") )
//        return InputDirector::parse();
    
    for( unsigned i = 0; i < files.size(); i++) {
        if(!strcmp(files[i],"--")){
            if(InputDirector::parse()!=0)
                return -1;
            continue;
        }
        FILE *file = fopen(files[i], "r");
        if(file) {
            int res = parse(files[i], file);
            fclose(file);
            if(res != 0)
                return res;
        }
        else {
            yyerror(*this,"Can't open input");
            return -1;
        }
    }
    return 0;
}

int
InputDirector::parse(
    int filesSize,
    const char **files ) 
{
    for(int i = 1; i < filesSize; i++) {
        FILE *file = fopen(files[i], "r");
        if(file) {
            int res = parse(files[i], file);
            fclose(file);
            if(res != 0)
                return res;
        }
        else {
            yyerror(*this,"Can't open input");
            return -1;
        }
    }
    return 0;
}

int
InputDirector::parse(
    const char* fileName,
    FILE *file ) 
{
	pv.clear();
    yyin = file;
    parserFile = fileName;
    parserLine = 1;
    yyparse(*this);

    if( parserErrors > 0 ) {
        cerr << "Aborting due to parser errors." << endl;
        return -1;
    }
    return 0;
}


int
InputDirector::parse( ) 
{
	pv.clear();
    yyin = stdin;
    parserFile = "STDIN";
    parserLine = 1;
    yyparse(*this);

    if( parserErrors > 0 ) {
        cerr << "Aborting due to parser errors." << endl;
        return -1;
    }
    return 0;
}

int 
InputDirector::onError( 
    const char* msg ) // Error handler for YACC
{
	if(findError) return 0;

	findError=true;

    parserErrors++;

    if( !parserStateInternal )
        {
        if(parserFile && strlen(parserFile) > 0 )
            cerr << parserFile << ": ";
        cerr << "line " << parserLine;

        cerr << ": ";
        }
    else
        cerr << "Internal parser invocation: ";

    cerr << msg << "." << endl;

    return 0;
}
