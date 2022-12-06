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
 * Term.cpp
 *
 *  Created on: Jun 18, 2015
 *      Author: jessica
 */

#include "Term.h"
#include "../table/TermTable.h"

namespace DLV2{
namespace grounder{

bool Term::isTermMax() const {return this->getIndex()==TermTable::getInstance()->term_max->getIndex();}
bool Term::isTermMin() const {return this->getIndex()==TermTable::getInstance()->term_min->getIndex();}
bool Term::isTermZero() const {return this->getIndex()==TermTable::getInstance()->term_zero->getIndex();}

}}
