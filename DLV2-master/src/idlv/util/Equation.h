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


#ifndef SRC_UTIL_EQUATION_H_
#define SRC_UTIL_EQUATION_H_

namespace DLV2 {

struct VAR{
	VAR(const VAR& v):i(v.i){};
	VAR(float i):i(i){};

	float i;
};

struct LINE{//k*x+a
	float a, k;

	LINE():a(0),k(0){}
	LINE(int a) : a(a), k(0){}
	LINE(VAR v) : a(0), k(1){}
	LINE(const LINE& l):a(l.a),k(l.k){}


LINE operator + (const LINE& B){//assumes that A.x == 0 or B.x == 0 or A.x == B.x
	LINE R;
	R.a = a + B.a;
	R.k = k + B.k;
	return R;
}

LINE operator - (const LINE& B){//same as +
	LINE R;
	R.a = a - B.a;
	R.k = k - B.k;
	return R;
}

LINE operator * (const LINE& B){//assumes that A.x == 0 or B.x == 0
	LINE R;
	R.a = a * B.a;
	R.k = k * B.a + B.k * a;
	return R;
}

LINE operator / (const LINE& B){//assumes that B.x == 0
	LINE R;
	R.a = a / B.a;
	R.k = k / B.a;
	return R;
}

VAR evaluate(const LINE& B){
	LINE C = (*this) - B;
	VAR x(-C.a/C.k);
	return x;
}

};
} /* namespace DLV2 */

#endif /* SRC_UTIL_EQUATION_H_ */
