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
 * ExtAtom.h
 *
 */

#ifndef EXTATOM_H_
#define EXTATOM_H_

#include "Atom.h"
namespace DLV2 {
namespace grounder{

class ExtAtom: public Atom {
public:
	ExtAtom( Predicate* predicate, vector< Term* >& terms, int separator, bool naf );
	ExtAtom( Predicate* predicate, int separator, bool naf );
	virtual ~ExtAtom(){};

	//TODO da vedere
	virtual size_t hash() { return 0; }

	virtual Atom* clone() = 0;

	/*
	 * Print any ExtAtom with its format &name(i1,...,in;o1,...,om)
	 * where i={set of input variables}, o={set of output variables}
	 */
	void print(ostream& stream) const;

	/*
	 * Two external atoms are equals if they are both external, if
	 * their predicate name, arity, separator position and terms are equals.
	 */
	bool operator==(const Atom& a) const;

	void ground(var_assignment& substritutionTerm,
			Atom*& templateAtom) { delete templateAtom; templateAtom = clone(); };

	bool isExternalAtom() const { return true; }

	unsigned getSeparator() const { return separator; }

	///This method Compute and returns the set of variables contained in the input parameters of the external atom
	virtual const set_term getInputVariables() const{
		set_term vars;
		for(unsigned i=0;i<separator;i++)
			terms[i]->getVariable(vars);
		return vars;
	}

	///This method Compute and returns the set of variables contained in the output parameters of the external atom
	virtual const set_term getOutputVariables() const{
		set_term vars;
		for(unsigned i=separator;i<getTermsSize();i++)
			terms[i]->getVariable(vars);
		return vars;
	}

	///Set if this atom is negated with negation as failure
	void setNegative(bool negative) {this->negative=negative;}

	///Returns true if the atom is negated with negation as failure
	bool isNegative() const {return negative;}

	//Template method
	bool match( var_assignment& assigment, bool direction );

	Predicate* getPredicate() const {return predicate;}


	protected:

		///If necessary this method transforms t into a ground term obtained by
		///replacing its variables with the constant values in the current assignment
		virtual bool getInputElem( var_assignment& assignment, Term*& t );
		/*
		 *If necessary this method transforms t into a ground list term obtained by
		 *replacing its variables with the constant values in the current assignment
		 */
		bool getInputList( var_assignment& assignment, Term*& t );
		/*
		 *If necessary this method transforms t into a ground numeric constant term obtained
		 *by replacing its variables with the constant values in the current assignment
		 */
		bool getInputNumericTerm( var_assignment& assignment, Term*& t );

		/*
		 * This method finds the numeric value of the term in the current assignment.
		 */
		bool findNumericValue( Term* t, var_assignment& assignment, int & value );
		/*
		 * This method finds the string value of the term in the current assignment.
		 * If term has a numeric value then converts it into a string
		 */
		bool findStringValue( Term* t, var_assignment& assignment, string & value );
		/*
		 * This method finds in the current assignment the value of terms string or symbolic constant.
		 */
		bool findOnlyStringValue( Term* t, var_assignment& assignment, string & value );
		/*
		 * Builds the output table of results by performing its own task
		 * on the bounded variables (input variables).
		 */
		virtual bool firstMatch( var_assignment& assignement ) = 0;
		// Checks if t is a constant term
		inline bool isConstantTerm( TermType t ){
			return t == TermType::NUMERIC_CONSTANT || t == TermType::STRING_CONSTANT || t == TermType::SYMBOLIC_CONSTANT;
		}
		// Checks if t is a list term
		inline bool isList( const TermType& t ){
			return t == TermType::LIST || t == TermType::LIST_HEAD_TAIL;
		}

		/*
		 *  Make the next match by mapping in the var_assignement
		 *  the next value on the results table.
		 */
		virtual bool nextMatch( var_assignment& assignement ) = 0;

		///Atom predicate
		Predicate* predicate;

		/*
		 * Indicates the position of the possible semicolon-separator that
		 * separates the input bounded values from those output.
		 * Example &a(X,Y;Z) --> separator = 2
		 */
		unsigned separator;

		///Negated with naf
		bool negative;

		TermTable * termTable;

};

} /* namespace DLV2 */
} /* namespace grounder */

#endif /* EXTATOM_H_ */
