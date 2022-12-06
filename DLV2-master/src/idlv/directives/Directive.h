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

/* 
 * File:   Directive.h
 * Author: cesco
 *
 * Created on 29 maggio 2014, 23.10
 */

#ifndef DIRECTIVE_H
#define	DIRECTIVE_H

#include <iostream>
using namespace std;

namespace DLV2 {

class Directive {
    public:
        Directive( char* n, char* v ): name(n), value(v) { }
        Directive( const char* n, const char* v ): name(n), value(v) { }
        Directive( char* v ): value(v) { }
        Directive( const char* v ): value(v) { }
        virtual ~Directive() {}
        friend inline ostream& operator<< ( ostream&, const Directive& );

    protected:
        string name;
        string value;
    };
    
    inline ostream& operator<< (ostream& out, const Directive& d )
    {
        out << d.name << d.value;
        return out;
    }

}

#endif	/* DIRECTIVE_H */

