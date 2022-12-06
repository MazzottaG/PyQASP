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
 * Timer.h
 *
 *  Created on: Feb 6, 2015
 *      Author: davide
 */

#ifndef SRC_UTIL_TIMER_H_
#define SRC_UTIL_TIMER_H_

#include <unordered_map>
#include <string>
#include <time.h>
#include <iostream>

namespace DLV2 {
namespace grounder {

using namespace std;

class Timer {
public:
	virtual ~Timer(){};

	void start(string label){
		clock_t clock_start=clock();
		auto it=map_clock.find(label);
		if(it==map_clock.end())
			map_clock.insert({label,clock_start});
		else{
			map_clock.erase(label);
			map_clock.insert({label,clock_start});
		}

	}

	inline void start(){
		temp_clock=clock();
	}

	clock_t getClock(){
		clock_t cl=clock();
		return cl;
	}

	void stop(string label){
		clock_t clock_end=clock();
		clock_t clock_start=map_clock[label];
		clock_t time_elapsed= clock_end - clock_start ;

		auto it=map_elapse_time.find(label);
		if(it==map_elapse_time.end())
			map_elapse_time.insert({label,time_elapsed});
		else{
			double previus_time=(*it).second;
			map_elapse_time.erase(label);
			map_elapse_time.insert({label,previus_time+time_elapsed});
		}
	}

	inline clock_t stop(){
		return (clock()-temp_clock);
	}

	void add(string label,clock_t time_elapsed){
		auto it=map_elapse_time.find(label);
		if(it==map_elapse_time.end())
			map_elapse_time.insert({label,time_elapsed});
		else{
			double previus_time=(*it).second;
			map_elapse_time.erase(label);
			map_elapse_time.insert({label,previus_time+time_elapsed});
		}
	}

	void print(){
		cerr<<"TIME:"<<endl;
		for(auto& time_pair:map_elapse_time)
			cerr<<time_pair.first<<" "<<(time_pair.second/ (double) CLOCKS_PER_SEC)<<endl;
	}

	void clear(){
		map_clock.clear();
		map_elapse_time.clear();
	}

	static Timer* getInstance(){
		if(timer_instance==nullptr)
			timer_instance=new Timer;
		return timer_instance;
	}

	static void printClock(string label){
		cerr<<label<<" "<<clock()<<endl;
	}

	static void printClockPerSecond(){
		cerr<<((double) CLOCKS_PER_SEC)<<endl;
	}

	static void printTimeElapsed(clock_t time, ostream& stream){
		stream<<"TIME: \t"<<((time)/(double) CLOCKS_PER_SEC)<<endl;
		stream<<"----------------------"<<endl;
	}

	void printSumTime(ostream& stream){
		stream<<"SUM TIME: \t"<<((sum_time)/(double) CLOCKS_PER_SEC)<<endl;
		stream<<"----------------------"<<endl;
	}

	inline void sumTime(clock_t time){
		timer_instance->sum_time+=time;
	}

private:
	Timer():sum_time(0),temp_clock(0){};

	static Timer* timer_instance;

	clock_t sum_time;

	clock_t temp_clock;
	unordered_map<string,clock_t> map_clock;
	unordered_map<string,clock_t> map_elapse_time;

};

} /* namespace grounder */
} /* namespace DLV2 */

#endif /* SRC_UTIL_TIMER_H_ */
