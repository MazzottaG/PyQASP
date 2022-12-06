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
 * HashString.h
 *
 *  Created on: 13/mar/2014
 *      Author: Davide
 */

#ifndef HASHSTRING_H_
#define HASHSTRING_H_

#include <string>
#include <iostream>
#include <functional>
#include <stdlib.h>
#include <ctime>

using namespace std;

namespace DLV2{

namespace grounder{


/**
 *  Abstract class that compute the hash of string
 *  The class is a singleton
 */
class HashString {
public:
	HashString(){};
	virtual ~HashString(){}
	///Compute the hash of a string
	virtual size_t computeHash(string& s) = 0;

	static void freeInstance(){ if(hashString!=0) delete hashString;}
	/// Return an hashString according the configuration
	static HashString* getHashString();
private:
	static HashString *hashString;
};

/**
 *  Compute the hash  with stl method
 */
class STLHashString: public HashString{
public:
	STLHashString(){};
	virtual size_t computeHash(string& s);
};

/**
 *  Compute the hash  with boost method
 */
class BOOSTHashString: public HashString{
public:
//	BOOSTHashString(){};
//	virtual size_t computeHash(string& s);

};

/**
 *  Compute the hash  with java method
 */
class JavaHashString: public HashString{
public:
	JavaHashString(){};
	virtual size_t computeHash(string& s);

};

/**
 *  Compute the hash  with perl Bernstein method
 */
class PerlBernsteinHashString: public HashString{
public:
	PerlBernsteinHashString(){};
	virtual size_t computeHash(string& s);

};

/**
 *  Compute the hash  with perl Jenkins method
 */
class PerlJenkinsHashString: public HashString{
public:
	PerlJenkinsHashString(){};
	virtual size_t computeHash(string& s);

};

/**
 *  Compute the hash  with murmur method
 */
class MurMurHashString: public HashString{
public:
	MurMurHashString(){srand (time(NULL));seed=rand();};
	virtual size_t computeHash(string& s);
private:
	int seed;
};


};

};

#endif /* HASHSTRING_H_ */
