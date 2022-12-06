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
 * HashString.cpp
 *
 *  Created on: 13/mar/2014
 *      Author: Davide
 */

#include "HashString.h"
#include "../../../common/Options.h"


#include <math.h>

namespace DLV2{

namespace grounder{

HashString *HashString::hashString;

size_t STLHashString::computeHash(string& s){
	  hash<string> str_hash;
	  return str_hash(s);
}

//size_t BOOSTHashString::computeHash(string& s) {
//	 boost::hash<string> str_hash;
//
//	 return str_hash(s);
//}

size_t JavaHashString::computeHash(string& s) {
		const char* sx=s.c_str();
		int length=s.length();
		size_t hash=0;
		for (int i = 0; i < length; i++) {
			hash+=sx[i]*pow(31.0,length-(i));
		}

		return hash;
}

size_t PerlBernsteinHashString::computeHash(string& s) {
		const char* sx=s.c_str();
		int i_hash=s.length();
		size_t hash_perl=0;
		while(i_hash--)
			hash_perl=(hash_perl*33) + *sx++;
		return hash_perl + (hash_perl>>5);
}

size_t PerlJenkinsHashString::computeHash(string& s) {
	const char* sx=s.c_str();
	size_t hash, i;
    for(hash = i = 0; i < s.length(); ++i)
    {
        hash += sx[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

size_t MurMurHashString::computeHash(string& s) {
	const uint64_t m = 0xc6a4a7935bd1e995;
	const int r = 47;
	const int len=s.size();

	uint64_t h = seed ^ (len * m);

	const uint64_t * data = (const uint64_t *)s.c_str();
	const uint64_t * end = data + (len/8);

	while(data != end)
	{
		uint64_t k = *data++;

		k *= m;
		k ^= k >> r;
		k *= m;

		h ^= k;
		h *= m;
	}

	const unsigned char * data2 = (const unsigned char*)data;

	switch(len & 7)
	{
	case 7: h ^= uint64_t(data2[6]) << 48;break;
	case 6: h ^= uint64_t(data2[5]) << 40;break;
	case 5: h ^= uint64_t(data2[4]) << 32;break;
	case 4: h ^= uint64_t(data2[3]) << 24;break;
	case 3: h ^= uint64_t(data2[2]) << 16;break;
	case 2: h ^= uint64_t(data2[1]) << 8;break;
	case 1: h ^= uint64_t(data2[0]);
	        h *= m;
	};

	h ^= h >> r;
	h *= m;
	h ^= h >> r;

	return h;
}

HashString* HashString::getHashString() {

	if(hashString==nullptr){
		switch (Options::globalOptions()->getHashType()) {
		case STL_HASH:
			hashString = new STLHashString;
			break;
//		case BOOST_HASH:
//			hashString = new BOOSTHashString;
//			break;
		case JAVA_HASH:
			hashString=new JavaHashString;
			break;
		case MUR_HASH:
			hashString=new MurMurHashString;
			break;
		case PERL_DJ:
			hashString=new PerlJenkinsHashString;
			break;
		case PERL_B:
			hashString=new PerlBernsteinHashString;
			break;

		default:
			hashString = new STLHashString;
			break;
		}
	}
	return hashString;
}

}

}
