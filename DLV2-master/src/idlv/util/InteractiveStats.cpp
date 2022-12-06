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
 * InteractiveStats.cpp
 *
 *  Created on: Mar 12, 2016
 *      Author: david
 */

#include "InteractiveStats.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

namespace DLV2 {
namespace grounder {

#ifdef NCURSES
string getTextSpace(string text,unsigned col){
	  unsigned plus=0;
	  for(char c:text)if(c=='%'){plus++;break;}
	  string spacePre="";
	  while(spacePre.size()<col)spacePre+=" ";
	  string spacePost="";
	  while(spacePre.size()+text.size()+spacePost.size()-plus<(unsigned)COLS)spacePost+=" ";
	  return spacePre+text+spacePost;
}


unsigned findCol(const vector<TableInfo>& table, TableInfo& t) {
	unsigned col = 0;
	if (t.father != 0) {
		unsigned root = t.father;
		while (root != 0) {
			col++;
			root = table[root - 1].father;
		}
	}
	return col;
}

bool haveChild(const vector<TableInfo>& table, unsigned i) {
	for(auto&t :table){
		if(t.father==i)
			return true;
	}
	return false;
}

void retractChild (vector<TableInfo>& table,unsigned father){

	for(unsigned i=0;i<table.size();i++){
		if(table[i].father==father){
			table[i].show=false;
			retractChild (table,i+1);
		}
	}

}

void expandRetractCursor (vector<TableInfo>& table,unsigned cursor){
	for(unsigned i=0;i<table.size();i++){
		if(table[i].father==cursor+1){
			table[i].show=!table[i].show;
			if(!table[i].show)
				retractChild(table,i+1);
		}

	}
//	if(!show)
//		expandRetractChild(table,expand);

}

unsigned getNextShow(vector<TableInfo>&table,unsigned index){
	for(unsigned i=index+1;i<table.size();i++){
		auto& t=table[i];
		if(t.show)
			return i;
	}
	return index;
}
unsigned getPreviousShow(vector<TableInfo>&table,unsigned index){
	if(index==0)return index;
	for(unsigned i=index-1;i>=0;i--){
		auto& t=table[i];
		if(t.show)
			return i;
	}
	return index;
}

bool isNotDisplayed(vector<TableInfo>&table,unsigned cursor,unsigned start,unsigned lineDisplay){
	for(unsigned i=start;i<start+lineDisplay;i++){
		auto& t=table[i];
		if(i==cursor)
			return false;
	}
	return true;
}
void InteractiveStats::displayStats(vector<TableInfo>&table,string heading1,string heading2){

//	initscr();
    SCREEN* s = NULL;
	FILE* out = stdout;
	if(!isatty(fileno(stdout))) {
		out = fopen("/dev/tty", "w");
		// Should really test `out` to make sure that worked.
		setbuf(out, NULL);
	}
	s = newterm(NULL, out, stdin);

	start_color();
	curs_set(0);
	noecho();
	keypad(stdscr, TRUE);
	init_pair(1,COLOR_WHITE,COLOR_BLACK);
	init_pair(2,COLOR_BLACK,COLOR_WHITE);
	init_pair(3,COLOR_WHITE,COLOR_BLUE);

	unsigned cursor=0;
	bool finish=false;
	int key=0;
	unsigned start=0;
	while(!finish){

		unsigned lineDisplay=0;
		attron(COLOR_PAIR(3));
		mvprintw(0,0,getTextSpace(heading1,0).c_str());
		mvprintw(1,0,getTextSpace(heading2,0).c_str());
		unsigned line=2;
		int first=-1;
		int last=-1;
		for(unsigned i=start;i<table.size();i++){
			if((int)line>=LINES-1)break;
			TableInfo& t=table[i];
			if(!t.show)continue;
			if(first==-1)first=i;
			last=i;
			if(i==cursor)
				attron(COLOR_PAIR(2));
			else
				attron(COLOR_PAIR(1));

			unsigned col = findCol(table, t);
			string child=(haveChild(table,i+1))?"+  ":"   ";

			mvprintw(line,0,getTextSpace(child+t.label,col).c_str());
			line++;
			lineDisplay++;
		}
		string info=to_string(cursor)+" "+to_string(key)+" "+to_string(lineDisplay)+" "+to_string(start)+" "+to_string(first)+" "+to_string(last);
		mvprintw(LINES-1,0,info.c_str());
		key=getch();
		switch (key) {
			case KEY_UP:
				cursor=getPreviousShow(table,cursor);
				if((int)cursor<first)
					start=getPreviousShow(table,start);

				break;
			case KEY_DOWN:
				cursor=getNextShow(table,cursor);
				if((int)cursor>last)
					start=getNextShow(table,start);

				break;
			case 'q':
				finish=true;
				break;
			case 10:
				expandRetractCursor(table,cursor);
				break;
			default:
				break;
		}
		clear();

		//timeout(500);
		refresh();
	}
	endwin();
    delscreen(s);

}

#endif

} /* namespace grounder */
} /* namespace DLV2 */
