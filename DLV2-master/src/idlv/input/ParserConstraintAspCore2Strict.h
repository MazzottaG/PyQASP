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
 * File:   ParserConstraintAspCore2Strict.h
 * Author: cesco
 *
 * Created on 3 giugno 2014, 18.40
 */

#ifndef PARSERCONSTRAINTASPCORE2STRICT_H
#define	PARSERCONSTRAINTASPCORE2STRICT_H

#include "ParserConstraint.h"

namespace DLV2
{

    class ParserConstraintAspCore2Strict : public ParserConstraint{
    public:
        ParserConstraintAspCore2Strict() {}
        virtual ~ParserConstraintAspCore2Strict() {}
        
        inline virtual void rangeFacts();
        inline virtual void directives();
    };
};

void
DLV2::ParserConstraintAspCore2Strict::rangeFacts()
{
    DLV2::ErrorMessage::errorDuringParsing("range facts are not in the ASPCore2 standard!");
}

void
DLV2::ParserConstraintAspCore2Strict::directives()
{
    DLV2::ErrorMessage::errorDuringParsing("directives are not in the ASPCore2 standard!");
}

#endif	/* PARSERCONSTRAINTASPCORE2STRICT_H */

