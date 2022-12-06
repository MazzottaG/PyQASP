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
 * File:   Utils.h
 * Author: cesco
 *
 * Created on 16 maggio 2014, 16.32
 */

#ifndef UTILS_H
#define	UTILS_H

#include <string>
#include <regex>
#include <sstream>

namespace DLV2
{

	enum TypeConv {
		/** UNSIGNED INTEGER */
		U_INT = 0,

		/** UNSIGNED TRUNCATED INTEGER */
		UT_INT,

		/** UNSIGNED ROUNDED INTEGER */
		UR_INT,

		/** UNSIGNED TRUNCATED INTEGER */
		T_INT,

		/** UNSIGNED ROUNDED INTEGER */
		R_INT,

		/** SYMBOLIC STRING */
		_CONST,

		/** QUOTED STRING */
		Q_CONST
	};

    class Utils {
    public:
        Utils() {}
        ~Utils() {}

        /** Convert a string, representing a numeric value,
         * to a long int variable.
         * @param value The long int variable to be converted
         * @param result The result of the conversion
         * @return true if the conversion was successfull
         */
        static bool parseLongInteger( const char* value, long int& result );
        /** Convert a string, representing a numeric value,
         * to an int variable.
         * @param value The long int variable to be converted
         * @param result The result of the conversion
         * @return true if the conversion was successfull
         */
        static bool parseInteger( const char* value, int& result );
        /** Check whether a string, representing a numeric value, is
         * valid number under base <baseN>
         * @param value The long int variable to be converted
         * @param result The result of the conversion
         * @return true if the check was ok
         */
        static bool isNumeric( const char* value, int baseN );

        /*
         * Check if set1 is contained in set2
         * @param set1 unordered_set
         * @param set2 unordered_set
         */
        template<class T>
        static bool isContained(const T& set1,const T& set2){
        	for(auto element:set1)
        		if(!set2.count(element))
        			return false;
        	return true;
        }

        /*
         * Check if set1 is disjoint with set2
         * @param set1 unordered_set
         * @param set2 unordered_set
         */
        template<class T>
        static bool isDisjoint(const T& set1,const T& set2){
        	for(auto element:set1)
        		if(set2.count(element))
        			return false;
        	return true;
        }

        /*
         * Do the union of set1 and set2 and put the result set in union
         * @param set1 unordered_set
         * @param set2 unordered_set
         * @param union_set unordered_set
         */
        template<class T>
        static void unionSet(const T& set1,const T& set2,T& union_set){
        	union_set.insert(set1.begin(),set2.begin());
        }

        /*
         * Do the intersection of set1 and set2 and put the result set in intersection_set
         * @param set1 unordered_set
         * @param set2 unordered_set
         * @param intersection_set unordered_set
         */
        template<class T>
        static bool intersectionSet(const T& set1,const T& set2,T& intersection_set){
        	bool isIntersection=false;
        	if(set1.size()<set2.size()){
            	for(auto element:set1)
            		if(set2.count(element)){
            			intersection_set.insert(element);
            			isIntersection=true;
            		}
        	}else {
            	for(auto element:set2)
            		if(set1.count(element)){
            			intersection_set.insert(element);
            			isIntersection=true;
            		}
        	}
        	return isIntersection;
        }

        /*
         * Do the difference of set1 and set2 and put the result set in difference
         * @param set1 unordered_set
         * @param set2 unordered_set
         * @param difference_set unordered_set
         */
        template<class T>
        static void differenceSet(const T& set1,const T& set2,T& difference_set){
			for(auto element:set1)
				if(!set2.count(element))
					difference_set.insert(element);
        }

        ///Split a string respect a char c and return the splitted string in a vector
        static std::vector<std::string> split(const std::string& input, const char c) {
        	std::vector<std::string> elems;
        	std::stringstream ss(input);
            std::string item;
            while (std::getline(ss, item, c)) {
                elems.push_back(item);
            }
            return elems;
        }

        static DLV2::TypeConv toTypeConv(const std::string& typeConv);

        static std::string toTypeConv(DLV2::TypeConv typeConv){
        	switch (typeConv) {
				case DLV2::TypeConv::U_INT:
					return "U_INT";
					break;
				case DLV2::TypeConv::UR_INT:
					return "UR_INT";
					break;
				case DLV2::TypeConv::UT_INT:
					return "UT_INT";
					break;
				case DLV2::TypeConv::_CONST:
					return "CONST";
					break;
				case DLV2::TypeConv::Q_CONST:
					return "Q_CONST";
					break;
				default:
					return "";
					break;
			}
        }

        static bool hasEnding (std::string const &fullString, std::string const &ending) {
			if (fullString.length() >= ending.length())
				return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));

			return false;
		}

        static bool hasStart (std::string const &fullString, std::string const &start) {
			if (fullString.length() >= start.length())
				return (0 == fullString.compare (0, start.length(), start));

			return false;
		}
    };

};

#endif	/* UTILS_H */
