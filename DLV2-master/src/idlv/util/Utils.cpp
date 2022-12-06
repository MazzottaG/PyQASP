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
#include "Utils.h"
#include <cstdlib>
#include <climits>
#include <sstream>

using namespace std;
using namespace DLV2;

bool
Utils::parseLongInteger(
    const char* value,
    long int& result ) 
{
    char* err;
 
    int n=strtol(value, &err, 10);
    if ( *err )
        return false;
    
    result = n;
    return true;
}

bool
Utils::parseInteger(
    const char* value,
    int& result ) 
{
    char* err;
 
    int n=strtol(value, &err, 10);
    if ( *err || n < INT_MIN || n > INT_MAX )
        return false;
    
    result = n;
    return true;
}

bool 
Utils::isNumeric( 
    const char* pszInput, 
    int nNumberBase )
{
    istringstream iss( pszInput );
 
    if ( nNumberBase == 10 )
    {
        double dTestSink;
        iss >> dTestSink;
    }
    else if ( nNumberBase == 8 || nNumberBase == 16 )
    {
        int nTestSink;
        iss >> ( ( nNumberBase == 8 ) ? oct : hex ) >> nTestSink;
    }
    else
        return false;
 
    // Was any input successfully consumed/converted?
    if ( !iss )
        return false;
 
    // Was all the input successfully consumed/converted?
    return ( iss.rdbuf()->in_avail() == 0 );
}

TypeConv Utils::toTypeConv(const string& typeConv) {
	if(typeConv == "U_INT")
		return DLV2::TypeConv::U_INT;
	else if(typeConv == "UT_INT")
		return DLV2::TypeConv::UT_INT;
	else if(typeConv == "UR_INT")
		return TypeConv::UR_INT;
	else if(typeConv == "CONST")
		return TypeConv::_CONST;
	else if(typeConv == "Q_CONST")
		return TypeConv::Q_CONST;
	else
		return TypeConv::Q_CONST;
}
