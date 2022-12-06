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
 * InteractiveStats.h
 *
 *  Created on: Mar 12, 2016
 *      Author: david
 */

#ifndef SRC_UTIL_INTERACTIVESTATS_H_
#define SRC_UTIL_INTERACTIVESTATS_H_

#include <string>
#include <vector>
#include <iostream>
#ifdef NCURSES
#include <ncurses.h>
#endif

using namespace std;

namespace DLV2 {
namespace grounder {

struct TableInfo{

	TableInfo(string label,bool show,	unsigned father):label(label),show(show),father(father){};

	string label;
	bool show;
	unsigned father;
};




#ifdef NCURSES


class InteractiveStats {
public:
	InteractiveStats(){};
	static void displayStats(vector<TableInfo>&table,string heading1,string heading2);
};



#else
class InteractiveStats {
public:
	InteractiveStats(){};
	static void displayStats(vector<TableInfo>&table,string heading1,string heading2){
		cerr<<"INTERACTIVE STATS NOT SUPPORTED"<<endl;
	};
};
#endif


} /* namespace grounder */
} /* namespace DLV2 */

#endif /* SRC_UTIL_INTERACTIVESTATS_H_ */
