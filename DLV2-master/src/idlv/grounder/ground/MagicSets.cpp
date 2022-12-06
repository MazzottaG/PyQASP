/*
 * File:   MagicSets.cpp
 * Author: pierfrancesco
 *
 * Created on 30/ott/2018, 16:58:27
 */

#include "MagicSets.h"
#include "../statement/Rule.h"

namespace DLV2{ namespace grounder{

ostream &
operator<<(
    ostream &out,
    const ADORNMENT_ITEM &item )
    {
    return item.print(out);
    }

ostream &
operator<<(
    ostream &out,
    const ADORNMENT &adornment )
    {
    assert( adornment.size == 0 || adornment.items != NULL );
    for( unsigned i = 0; i < adornment.size; i++ )
        {
        assert( adornment.items[i] != NULL );
        out << *adornment.items[i];
        }
    return out;
    }

ostream &
operator<<(
    ostream &out,
    const ADORNED_PREDICATE &predicate)
    {
    assert( predicate.getPredicate() != nullptr );
    return out << predicate.getPredicate()->getName()
               << ( predicate.adornment.getSize() > 0 ? MAGIC_ADORNMENT_SEPARATOR : "")
               << predicate.adornment;
    }

ostream &
operator<<(
    ostream &out,
    const ADORNED_ATOM &lit )
    {
    if( lit.atom == nullptr )
        return out;
    if( lit.atom->isClassicalLiteral() ) {
        ClassicalLiteral *cl = dynamic_cast<ClassicalLiteral*>(lit.atom);
        if(cl->isNegative())
            out<<"not ";
        if(cl->getPredicate()->isTrueNegated())
            out<<"-";
        out << cl->getPredicate()->getName() << ( lit.adornment.getSize() > 0 ? MAGIC_ADORNMENT_SEPARATOR : "")
            << lit.adornment;
        if( cl->getTermsSize() > 0 ) {
            out << "(";
            for( unsigned i=0; i<cl->getTermsSize(); i++ ) {
                cl->getTerm(i)->print(out);
                if(i!=cl->getTermsSize()-1)
                    out<<",";
            }
            out << ")";
        }
        return out;
    }
    out << lit.atom->toString();
    return out;
    }

ostream &
operator<<(
    ostream &out,
    const ADORNED_RULE &rule )
    {
    unsigned int i=0;
    for (auto atom:rule.getHead()) {
        out << atom->toString();
        if(i!=rule.getHead().size()-1)
            out<<SEPARATOR_HEAD;
        i++;
    }
    if( rule.getBody().size()>0 ){
        out<<":-";
        unsigned int i=0;
        for (auto atom:rule.getBody()) {
            out << atom->toString();
            if(i!=rule.getBody().size()-1)
                out<<SEPARATOR_BODY;
            i++;
        }
    }
    out<<"."<<endl;
    return out;
    }

ostream &
operator<<(
    ostream &out,
    const DECORATED_RULE &dr )
    {
    if( dr.rule != nullptr )
        {
        dr.rule->print(out);
        }
    out << "[Head Adornments]" << endl;
    for( auto p: dr.headAtomPositionToAdorment )
        out << p.first << ": " << *p.second << endl;
    out << "[Body Adornments]" << endl;
    for( auto p: dr.bodyAtomPositionToAdorment )
        out << p.first << ": " << *p.second << endl;
    return out;
    }

void
ADORNMENT_FREE::addBoundVariables(
    Predicate *pred,
    Term *,
    set_term &,
    binding_map &,
    bool comesFromHead ) const
    {
    }

void
ADORNMENT_FREE::addBoundTerms(
    Term *,
    vector<Term*> & ) const
    {
    }

void
ADORNMENT_BOUND::addBoundVariables(
    Predicate *pred,
    Term *term,
    set_term &boundVariables,
    binding_map &boundVarsToPred,
    bool comesFromHead  ) const
    {
    assert( term != nullptr );
    switch( term->getType() )
        {
        case TermType::VARIABLE:
            if( boundVariables.find(term) == boundVariables.end() )
                boundVarsToPred.insert(pair<Term*,Predicate*>(term,(comesFromHead ? nullptr : pred)));
            boundVariables.insert(term);
            break;

//////////////// MARIO
//        case TermType::FUNCTION:
//            term->getVariable(boundVariables);
//            break;
//////////////// END

        default:
            break;
        }
    }

void
ADORNMENT_BOUND::addBoundTerms(
    Term *termIn,
    vector<Term*> &termsOut ) const
    {
    assert( termIn != nullptr );
    termsOut.push_back(termIn);
    }

ADORNMENT::ADORNMENT(
    const Atom *atom )
    {
    assert( atom != nullptr );
    size = atom->getTermsSize();
    set_term tmp;
    binding_map map;
    StatementAtomMapping statementAtomMapping;
    init(atom,tmp,map,statementAtomMapping);
    }

ADORNMENT::ADORNMENT(
    const Atom *atom,
    const set_term &boundVariables,
    const binding_map &boundVarsToPred,
    StatementAtomMapping& statementAtomMapping )
    {
    assert( atom != nullptr );
    size = atom->getTermsSize();
    init(atom,boundVariables,boundVarsToPred,statementAtomMapping);
    }

void ADORNMENT::init(
    const Atom *atom,
    const set_term &boundVariables,
    const binding_map &boundVarsToPred,
    StatementAtomMapping& statementAtomMapping  )
    {
    assert( atom != nullptr );
    if( atom->getTermsSize() == 0 )
        {
        items = nullptr;
        assert( size == 0 );
        return;
        }

    if( !atom->isClassicalLiteral() || atom->getPredicate()->isEdb() )
        {
        items = nullptr;
        size = 0;
        return;
        }

    const vector<Term*> &params = atom->getTerms();

    items = new ADORNMENT_ITEM*[size];
    unsigned idx = 0;
    for( vector<Term*>::const_iterator i = params.begin();
         i != params.end();
         ++i, ++idx )
        {
        switch( (*i)->getType() )
            {
            case TermType::VARIABLE:
            case TermType::ANONYMOUS:
                {
                if( boundVariables.find(*i) != boundVariables.end() )
                    {
                    Predicate* pA = atom->getPredicate();
                    Predicate* pB = nullptr;
                    auto it = boundVarsToPred.find(*i);
                    if( it != boundVarsToPred.end() )
                        {
                        pB = it->second;
                        if( pA != nullptr && pB != nullptr )
                            {
                            trace_msg(grounding,2,"" << (*i)->toString() << " is bound by " << pB->getName() << endl);
                            if( statementAtomMapping.checkIfDepended(pB,pA) )
                                {
                                trace_msg(grounding,2,"Do not propagate: " << pB->getName() << " depends from " << pA->getName() << endl);
                                items[idx] = new ADORNMENT_FREE();
                                }
                            else
                                {
                                items[idx] = new ADORNMENT_BOUND();
                                Predicate *magicRepA = MAGIC_SETS::getMagicRep(pA);
                                statementAtomMapping.addPredicateDependencies(magicRepA,pB);
                                trace_msg(grounding,2,"Propagate and add dependency : " << magicRepA->getName() << " -> " << pB->getName() << endl);
                                }
                            break;
                            }
                        }
                    items[idx] = new ADORNMENT_BOUND();
                    }
                else
                    items[idx] = new ADORNMENT_FREE();
                break;
                }
            case TermType::FUNCTION:
                {
//////////////// OLD DLV
                if( (*i)->isGround() )
                    items[idx] = new ADORNMENT_BOUND();
                else
                    items[idx] = new ADORNMENT_FREE();
//////////////// end

//////////////// MARIO
//                set_term vars;
//                (*i)->getVariable(vars);
//                bool allBound = true;
//                for( auto v:vars )
//                    if( boundVariables.find(v) == boundVariables.end() ) {
//                        allBound = false;
//                        break;
//                    }
//                if( allBound )
//                    items[idx] = new ADORNMENT_BOUND();
//                else
//                    items[idx] = new ADORNMENT_FREE();
//////////////// END

                break;
                }

        case TermType::NUMERIC_CONSTANT:
        case TermType::STRING_CONSTANT:
        case TermType::SYMBOLIC_CONSTANT:
            items[idx] = new ADORNMENT_BOUND();
            break;

        default:
            assert( 0 );
            break;
            }
        }
    }

ADORNMENT::ADORNMENT(
    const ADORNMENT &init )
    : items( nullptr ),
      size( init.size )
    {
    cloneItems( init.items, init.size );
    }

ADORNMENT::~ADORNMENT()
    {
    freeItems();
    }

ADORNMENT &
ADORNMENT::operator=(
    const ADORNMENT &right )
    {
    cloneItems(right.items,right.size);
    return *this;
    }

bool
ADORNMENT::operator==(
    const ADORNMENT & a ) const
    {
    if( a.size != this->size )
        return false;
    if( size > 0 )
        {
        assert( a.items != nullptr && this->items != nullptr );
        for( unsigned i = 0; i < size; i++ )
            {
            assert( a.items[i] != nullptr && this->items[i] != nullptr );
            if( a.items[i]->isFullyBound() != this->items[i]->isFullyBound() )
                return false;
            }
        }
    else
        assert( a.items == nullptr && this->items == nullptr );
    return true;
    }

void
ADORNMENT::cloneItems(
    ADORNMENT_ITEM **const &init,
    unsigned initSize )
    {
    freeItems();
    assert( items == nullptr );

    size = initSize;
    if( size == 0 )
        {
        assert( init == nullptr );
        items = nullptr;
        return;
        }

    assert( init != nullptr );
    items = new ADORNMENT_ITEM*[ size ];
    for( unsigned i = 0; i < size; i++ )
        items[i] = init[i]->clone();
    }

void
ADORNMENT::freeItems()
    {
    if( items != nullptr )
        {
        for( unsigned i = 0; i < size; i++ )
            {
            assert( items[i] != nullptr );
            delete items[i];
            }
        delete[] items;
        size = 0;
        items = nullptr;
        }
    }

const ADORNMENT_ITEM &
ADORNMENT::operator[](
    unsigned i ) const
    {
    assert( i < size );
    assert( items != nullptr );
    assert( items[i] != nullptr );
    return *items[i];
    }

ADORNMENT_ITEM &
ADORNMENT::operator[](
    unsigned i )
    {
    assert( i < size );
    assert( items != nullptr );
    assert( items[i] != nullptr );
    return *items[i];
    }

string
ADORNMENT::toString() const
    {
    stringstream ss;
    assert( items != nullptr );
    for( unsigned i = 0; i < size; i++ )
        {
        assert( items[i] != nullptr );
        items[i]->print(ss);
        }
    return ss.str();
    }

bool
ADORNMENT::isFullyBound() const
    {
    assert( items != nullptr );
    for( unsigned i = 0; i < size; i++ )
        {
        assert( items[i] != nullptr );
        if( ! items[i]->isFullyBound() )
            return false;
        }
    return true;
    }

bool
ADORNMENT::isFullyFree() const
    {
    assert( size == 0 || items != nullptr );
    for( unsigned i = 0; i < size; i++ )
        {
        assert( items[i] != nullptr );
        if( ! items[i]->isFullyFree() )
            return false;
        }
    return true;
    }

bool
ADORNMENT::isSubsumedBy(
    const ADORNMENT& a ) const
    {
    if( size != a.size )
        return false;
    for( unsigned i=0; i<size; i++ )
        {
        if( this->items[i]->isFullyFree() && a.items[i]->isFullyBound() )
            return false;
        }
    return true;
    }

bool
ADORNMENT::isStrictlySubsumedBy(
    const ADORNMENT& a ) const
    {
    return (this->isSubsumedBy(a) && *this != a);
    }

void
ADORNMENT::addBoundVariables(
    Predicate *pred,
    const vector<Term*> &terms,
    set_term &boundVariables,
    binding_map &boundVarsToPred,
    bool comesFromHead ) const
    {
    for( unsigned i = 0; i < size; i++ )
        {
        assert( items[i] != nullptr );
        items[i]->addBoundVariables(pred,terms[i],boundVariables,boundVarsToPred,comesFromHead);
        }
    }

void
ADORNMENT::addBoundTerms(
    const vector<Term*> &termsIn,
    vector<Term*> &termsOut ) const
    {
    for( unsigned i = 0; i < size; i++ )
        {
        assert( items[i] !=  nullptr );
        items[i]->addBoundTerms(termsIn[i],termsOut);
        }
    }

size_t
ADORNMENT::hasher::operator()(
    const ADORNMENT &adornment ) const
    {
    size_t h = 0;
    for( unsigned i = 0; i < adornment.size; i++ )
        h = h * 29 + adornment[i].hash();
    return h;
    }

ADORNED_PREDICATE::ADORNED_PREDICATE(
    Predicate *predicate_,
    const ADORNMENT &adornment_ )
    : adornment(adornment_),
      predicate(predicate_)
    {
    assert( predicate_ != nullptr );
    }

string
ADORNED_PREDICATE::toString() const
    {
    assert( predicate != nullptr );
    stringstream ss;
    ss << this->predicate->getName() << MAGIC_ADORNMENT_SEPARATOR << adornment;
    return ss.str();
    }

size_t
ADORNED_PREDICATE::hash() const
    {
    static ADORNMENT::hasher adornmentHasher;
    return (predicate != nullptr ? predicate->hash() : 0) * 11 + adornmentHasher(adornment) * 61;
    }

bool
ADORNED_PREDICATE::operator==(
    const ADORNED_PREDICATE &ap ) const
    {
    if( this->predicate == ap.predicate && this->adornment == ap.adornment )
        return true;
    return false;
    }

size_t
ADORNED_PREDICATE::hasher::operator()(
    const ADORNED_PREDICATE &predicate ) const
    {
    static ADORNMENT::hasher adornmentHasher;
    return predicate.hash() * 11 + adornmentHasher(predicate.adornment) * 61;
    }

size_t
ADORNED_PREDICATE::hasher::operator()(
    ADORNED_PREDICATE *predicate ) const
    {
    static ADORNMENT::hasher adornmentHasher;
    return predicate->hash() * 11 + adornmentHasher(predicate->adornment) * 61;
    }

bool
ADORNED_PREDICATE::hasher::operator()(
    const ADORNED_PREDICATE &ap1,
    const ADORNED_PREDICATE &ap2 ) const
    {
    return ap1==ap2;
    }

bool
ADORNED_PREDICATE::hasher::operator()(
    ADORNED_PREDICATE *ap1,
    ADORNED_PREDICATE *ap2 ) const
    {
    return *ap1==*ap2;
    }

ABSTRACT_ADORNED_ATOM::ABSTRACT_ADORNED_ATOM(
    Atom *a )
    {
    assert( a != nullptr );
    atom = a->clone();
    }

AGGREGATE_ADORNED_ATOM::~AGGREGATE_ADORNED_ATOM()
	{
	assert( atom != nullptr && atom->isAggregateAtom());
    	for(unsigned i=0; i<atom->getAggregateElementsSize(); i++)
    		atom->getAggregateElement(i)->deleteAtoms();
	}

AGGREGATE_ADORNED_ATOM::AGGREGATE_ADORNED_ATOM(
	Atom* a,
	const set_term &boundVariables,
	const binding_map &boundVarsToPred,
	StatementAtomMapping &statementAtomMapping ):
		ABSTRACT_ADORNED_ATOM(a)
	{
	for(unsigned i=0; i<a->getAggregateElementsSize(); i++)
		{
 		adornments.push_back(
				ADORNMENT(
						a->getAggregateElement(i)->getNafLiteral(0),
						boundVariables,
						boundVarsToPred,
						statementAtomMapping));
		}
	}

AGGREGATE_ADORNED_ATOM::AGGREGATE_ADORNED_ATOM(
	Atom* atom ):
		ABSTRACT_ADORNED_ATOM(atom)
	{
	assert(atom != nullptr && atom->isAggregateAtom());
	for(unsigned i=0; atom->getAggregateElementsSize(); i++)
		{
		adornments.push_back(ADORNMENT(atom->getAggregateElement(i)->getNafLiteral(0)));
		}
	}

ABSTRACT_ADORNED_ATOM&
AGGREGATE_ADORNED_ATOM::operator=(
	const ABSTRACT_ADORNED_ATOM& right )
	{
    assert( atom != nullptr && atom->isAggregateAtom());
    const AGGREGATE_ADORNED_ATOM &tmp = dynamic_cast<const AGGREGATE_ADORNED_ATOM&>(right);
    for(unsigned i=0; i<atom->getAggregateElementsSize(); i++)
    	atom->getAggregateElement(i)->deleteAtoms();
    delete atom;
    atom = tmp.atom->clone();
    adornments = tmp.adornments;
    return *this;
	}

bool
AGGREGATE_ADORNED_ATOM::operator==(
	const ABSTRACT_ADORNED_ATOM & aA ) const
	{
	const AGGREGATE_ADORNED_ATOM &a = dynamic_cast<const AGGREGATE_ADORNED_ATOM&>(aA);
	if( this->adornments.size() != a.adornments.size() )
		return false;
	for( unsigned i=0; i<this->adornments.size(); i++ )
		{
		if( this->adornments[i] != a.adornments[i] )
			return false;
		}
	if( ! (this->atom == a.atom) )
			return false;
    return true;
	}

const ADORNMENT &
AGGREGATE_ADORNED_ATOM::getAdornment(
	unsigned index ) const
	{
	assert(index<adornments.size());
	return adornments[index];
	}

ADORNED_PREDICATE
AGGREGATE_ADORNED_ATOM::getAdornedPredicate(
	unsigned index ) const
	{
    assert_msg( (atom != nullptr && atom->isAggregateAtom()), "Not valid atom!" );
    assert_msg( (atom->getAggregateElementsSize() > index ), "Not valid atom!" );
    assert_msg( (adornments.size() > index ), "Not valid adornments!" );

    return ADORNED_PREDICATE( atom->getAggregateElement(index)->getNafLiteral(0)->getPredicate(), adornments[index] );
	}

void
AGGREGATE_ADORNED_ATOM::addBoundVariables(
	set_term &boundVariables,
	binding_map &boundVarsToPred,
	bool comesFromHead ) const
	{
    assert( atom != nullptr && atom->isAggregateAtom() );
    for( unsigned i=0; i<atom->getAggregateElementsSize(); i++ )
    	{
    	Atom* tmp = atom->getAggregateElement(i)->getNafLiteral(0);
    	if( tmp->getTermsSize() > 0 )
    		adornments[i].addBoundVariables(tmp->getPredicate(),tmp->getTerms(),boundVariables,boundVarsToPred,comesFromHead);
    	}
	}

Atom*
AGGREGATE_ADORNED_ATOM::createMagicAtom(
	unsigned index ) const
	{
    assert( atom != nullptr && atom->isAggregateAtom() && index < atom->getAggregateElementsSize() );
	Atom* tmp = atom->getAggregateElement(index)->getNafLiteral(0);
    stringstream ss;
    vector<Term*> params;
    ss << MAGIC_PREFIX << tmp->getPredicate()->getName();

    if( tmp->getTermsSize() > 0 )
        {
        ss << MAGIC_ADORNMENT_SEPARATOR << this->adornments[index];
        this->adornments[index].addBoundTerms(tmp->getTerms(),params);
        }
    Predicate *predicate = new Predicate();
    predicate->setArity(params.size());
    predicate->setIdb();
    predicate->setName(ss.str());
    if(predicate->getName()!=PREDNAME_QUERY)
        predicate->setHiddenForPrinting(true);
    else
        predicate->setHiddenForPrinting(false);
    PredicateTable::getInstance()->insertPredicate(predicate);
    PredicateExtTable::getInstance()->addPredicateExt(predicate);
    return new ClassicalLiteral(predicate,params,tmp->isHasMinus(),tmp->isNegative());
	}

string
AGGREGATE_ADORNED_ATOM::toString() const
	{
    assert( atom != nullptr && atom->isAggregateAtom() );
    stringstream stream;
	if(atom->isNegative())
		stream<<"not ";
	if(atom->getFirstGuard()!=nullptr){
		atom->getFirstGuard()->print(stream);
		stream<<atom->getBinopToStrng(atom->getFirstBinop());
	}
	stream<<"#";
	if(atom->getAggregateFunction()==AggregateFunction::COUNT){
		stream<<"count";
	}else if(atom->getAggregateFunction()==AggregateFunction::MIN){
		stream<<"min";
	}else if(atom->getAggregateFunction()==AggregateFunction::MAX){
		stream<<"max";
	}else if(atom->getAggregateFunction()==AggregateFunction::SUM){
		stream<<"sum";
	}
	stream<<"{";
	bool first=true;
	bool firstElement=true;
	for(unsigned i=0; i<atom->getAggregateElementsSize(); i++){
		if(!firstElement)stream<<";";else firstElement=false;
		for(auto& term:atom->getAggregateElement(i)->getTerms()){
			if(!first)stream<<",";else first=false;
			term->print(stream);
		}
		stream<<":";first=true;
		for(auto& naf:atom->getAggregateElement(i)->getNafLiterals()){
			if(!first)stream<<",";else first=false;
		    stream << naf->toString() << MAGIC_ADORNMENT_SEPARATOR << adornments[i];
		}
		first=true;
	}
	stream<<"}";
	if(atom->getSecondGuard()!=nullptr){
		stream<<atom->getBinopToStrng(atom->getSecondBinop());
		atom->getSecondGuard()->print(stream);
	}
    return stream.str();
	}

bool
AGGREGATE_ADORNED_ATOM::hasBoundLiterals() const
	{
	for( unsigned i=0; i<atom->getAggregateElementsSize(); i++ )
	{
		Atom* tmp = atom->getAggregateElement(i)->getNafLiteral(0);
		if( tmp->hasPredicate() && ! tmp->getPredicate()->isEdb() && ! adornments[i].isFullyFree() )
			return true;
	}
	return false;
	}

bool
AGGREGATE_ADORNED_ATOM::isFullyFree() const
	{
	for(unsigned i=0; i<adornments.size(); i++ )
		if( ! adornments[i].isFullyFree() )
			return false;
	return true;
	}

ADORNED_ATOM::ADORNED_ATOM(
    const ADORNED_PREDICATE &predicate,
    const vector<Term*> &params,
    bool hasMinus,
    bool negative )
    : adornment(predicate.getAdornment())
    {
    vector<Term*> terms = params;
    atom = new ClassicalLiteral(predicate.getPredicate(),terms,hasMinus,negative);
    }

ADORNED_ATOM::ADORNED_ATOM(
    Atom *a,
    const set_term &boundVariables,
    const binding_map &boundVarsToPred,
    StatementAtomMapping &statementAtomMapping )
    : adornment(a,boundVariables,boundVarsToPred,statementAtomMapping)
    {
    assert( a != nullptr );
    atom = a->clone();
    }

ADORNED_ATOM::ADORNED_ATOM(
    const ADORNED_ATOM & aa )
    : adornment(aa.adornment)
    {
    assert( aa.atom != nullptr );
    atom = aa.atom->clone();
    }

ADORNED_ATOM::ADORNED_ATOM(
	Atom *tmp,
	const ADORNMENT &ad ):
		adornment(ad)
	{
    assert( tmp != nullptr );
    atom = tmp->clone();
	}

ABSTRACT_ADORNED_ATOM &
ADORNED_ATOM::operator=(
    const ABSTRACT_ADORNED_ATOM& right )
    {
    assert( atom != nullptr );
    delete atom;
    const ADORNED_ATOM &tmp = dynamic_cast<const ADORNED_ATOM&>(right);
    atom = tmp.atom->clone();
    adornment = tmp.adornment;
    return *this;
    }

bool
ADORNED_ATOM::operator==(
    const ABSTRACT_ADORNED_ATOM & aA ) const
    {
	const ADORNED_ATOM &a = dynamic_cast<const ADORNED_ATOM&>(aA);
    if( this->adornment == a.adornment )
        {
        if( this->atom == a.atom )
            return true;
        else if( this->atom != nullptr && a.atom != nullptr && *this->atom == *a.atom )
            return true;
        }
    return false;
    }

//////// TODO: check correctness of printing ss << *atom << "#" << adornment;
//////// what happens if *atom is already adorned? in any case, the adornment is printed after the term list..
string
ADORNED_ATOM::toString() const
    {
    assert( atom != nullptr );
    stringstream ss;
    ss << atom->toString() << MAGIC_ADORNMENT_SEPARATOR << adornment;
    return ss.str();
    }

void
ADORNED_ATOM::addBoundVariables(
    set_term &boundVariables,
    binding_map &boundVarsToPred,
    bool comesFromHead ) const
    {
    assert( atom != nullptr );
    if( atom->getTermsSize() > 0 )
        adornment.addBoundVariables(atom->getPredicate(),atom->getTerms(),boundVariables,boundVarsToPred,comesFromHead);
    }

Atom*
ADORNED_ATOM::createMagicAtom(
	unsigned ) const
    {
    assert( atom != nullptr && atom->hasPredicate() );
    stringstream ss;
    vector<Term*> params;
    ss << MAGIC_PREFIX << atom->getPredicate()->getName();

    if( atom->getTermsSize() > 0 )
        {
        ss << MAGIC_ADORNMENT_SEPARATOR << this->adornment;
        this->adornment.addBoundTerms(atom->getTerms(),params);
        }
    Predicate *predicate = new Predicate();
    predicate->setArity(params.size());
    predicate->setIdb();
    predicate->setName(ss.str());
    if(predicate->getName()!=PREDNAME_QUERY)
        predicate->setHiddenForPrinting(true);
    else
        predicate->setHiddenForPrinting(false);
    PredicateTable::getInstance()->insertPredicate(predicate);
    PredicateExtTable::getInstance()->addPredicateExt(predicate);
    return new ClassicalLiteral(predicate,params,atom->isHasMinus(),atom->isNegative());
    }

bool
ADORNED_ATOM::hasBoundLiterals() const
	{
	if( atom->hasPredicate() && ! atom->getPredicate()->isEdb() && ! adornment.isFullyFree() )
		return true;
	return false;
	}

ADORNED_PREDICATE
ADORNED_ATOM::getAdornedPredicate(
	unsigned ) const {
    assert_msg( (atom != nullptr && atom->hasPredicate()), "Not valid atom!" );
    return ADORNED_PREDICATE( atom->getPredicate(), adornment );
    }

ADORNED_RULE::~ADORNED_RULE()
	{
	for( ABSTRACT_ADORNED_ATOM* a : head )
		delete a;
	for( ABSTRACT_ADORNED_ATOM* a : body )
		delete a;
	}

bool
ADORNED_RULE::hasBoundLiterals() const
    {
    for( BODY::const_iterator i = body.begin(); i != body.end(); i++ )
        if( (*i)->hasBoundLiterals() )
            return true;
    return false;
    }

DECORATED_RULE::DECORATED_RULE(
    const DECORATED_RULE &dr )
    {
    rule = dr.rule;
    for( auto p: dr.headAtomPositionToAdorment )
        {
        assert( p.second != nullptr );
        headAtomPositionToAdorment[p.first] = new ADORNMENT(*p.second);
        }
    for( auto p: dr.bodyAtomPositionToAdorment )
        {
        assert( p.second != nullptr );
        bodyAtomPositionToAdorment[p.first] = new ADORNMENT(*p.second);
        }
    }

DECORATED_RULE::~DECORATED_RULE()
    {
    for( auto p: headAtomPositionToAdorment )
        {
        assert( p.second != nullptr );
        delete p.second;
        }
    for( auto p: bodyAtomPositionToAdorment )
        {
        assert( p.second != nullptr );
        delete p.second;
        }
    }

DECORATED_RULE &
DECORATED_RULE::operator=(
    const DECORATED_RULE & dr )
    {
    rule = nullptr;
    for( auto p: headAtomPositionToAdorment )
        {
        assert( p.second != nullptr );
        delete p.second;
        }
    for( auto p: bodyAtomPositionToAdorment )
        {
        assert( p.second != nullptr );
        delete p.second;
        }
    rule = dr.rule;
    for( auto p: dr.headAtomPositionToAdorment )
        {
        assert( p.second != nullptr );
        headAtomPositionToAdorment[p.first] = new ADORNMENT(*p.second);
        }
    for( auto p: dr.bodyAtomPositionToAdorment )
        {
        assert( p.second != nullptr );
        bodyAtomPositionToAdorment[p.first] = new ADORNMENT(*p.second);
        }
    return *this;
    }

void
DECORATED_RULE::pushToHead(
    Atom *atom,
    const ADORNMENT *adornment )
    {
    assert( rule != nullptr );
    rule->addInHead(atom);
    if( adornment != nullptr )
        headAtomPositionToAdorment[rule->getSizeHead()-1] = new ADORNMENT(*adornment);
    }

void
DECORATED_RULE::pushToBody(
    Atom *atom,
    const ADORNMENT *adornment )
    {
    assert( rule != nullptr );
    rule->addInBody(atom);
    if( adornment != nullptr )
        bodyAtomPositionToAdorment[rule->getSizeBody()-1] = new ADORNMENT(*adornment);
    }

ADORNMENT *
DECORATED_RULE::getHeadAtomAdornment(
    unsigned pos )
    {
    auto it = headAtomPositionToAdorment.find(pos);
    if( it == headAtomPositionToAdorment.end() )
        return nullptr;
    else
        return it->second;
    }

ADORNMENT *
DECORATED_RULE::getBodyAtomAdornment(
    unsigned pos )
    {
    auto it = bodyAtomPositionToAdorment.find(pos);
    if( it == bodyAtomPositionToAdorment.end() )
        return nullptr;
    else
        return it->second;
    }

MAGIC_SETS::MAGIC_SETS(
    std::vector<Rule*> &idb,
    const std::vector<Rule*> &constraints,
    const std::vector<Rule*> &wconstraints,
    std::vector<Atom*> *query,
    bool isqueryground,
	const StatementAtomMapping& statementAtomMapping)
    : IDB(idb),
    Constraints(constraints),
    WConstraints(wconstraints),
    Query(query),
    IsQueryGround(isqueryground),
	statementAtomMapping(statementAtomMapping)
    {
    assert( ! IDB.empty() );
    for( size_t i = 0; i < IDB.size(); i++ )
        {
        assert( IDB[i] != nullptr );
        const vector<Atom*> &head = IDB[i]->getHead();
        assert( head.size() > 1 || IDB[i]->getSizeBody() > 0 );

        for( vector<Atom*>::const_iterator j = head.begin();
                     j != head.end();
                     j++ )
            {
            assert( (*j)->hasPredicate() );
            PREDICATES_RULES_INDEX::iterator k =
                predicatesRulesIndex.find((*j)->getPredicate());
            if( k == predicatesRulesIndex.end() )
                {
                vector<size_t> tmp;
                tmp.push_back(i);
                predicatesRulesIndex.insert(
                    PREDICATES_RULES_INDEX::value_type(
                            (*j)->getPredicate(),
                            tmp
                    )
                );
                }
            else
                {
                k->second.push_back(i);
                }
            }
        }
    }

MAGIC_SETS::~MAGIC_SETS()
    {
    for( auto adornments: predicateMostGeneralAdornments )
        for( auto ad: adornments.second )
            if( ad != nullptr )
                delete ad;
    }

void
MAGIC_SETS::rewrite()
    {
    assert( Query );

    trace_msg(grounding,1,"-------    Magic Sets Rewriting   -------" << endl << endl);
/////////////////// TODO: chiedi a Mario perché poteva accadere che la regola venisse eliminata e chiedi a Jessica cosa succede ora in iDLV
    if( ! IsQueryGround && predicatesRulesIndex.find(PredicateTable::getInstance()->getQueryPredicate()) == predicatesRulesIndex.end() )
        {
        trace_msg(grounding,1,"Skip Magic Sets Rewriting because #query rule was removed." << endl);
        return;
        }

    trace_action(grounding,1,{
        cerr << "Original Rules" << endl;
        for( auto r: IDB ) r->print(cerr); cerr << endl;
        for( auto c: Constraints ) c->print(cerr); cerr << endl;
        for( auto wc: WConstraints ) wc->print(cerr); cerr << endl;
        });

    generateMagicPredsAndAddDeps();

    if( ! processQuery() )
        {
        trace_msg(grounding,2,"Not rewriting the query. Magic Sets are not applied!"<< endl << endl);
        return;
        }

    for(vector<Rule*>::const_iterator it = Constraints.begin(); it != Constraints.end(); ++it)
        {
        // 1) build the rule and adorn it
        assert( boundVarsToPred.empty() );
        assert( boundVariables.empty() );
        ADORNED_RULE aR;
        int permissibles = adornBody(
            (*it)->getBody(),
            false,
            aR
        );
        //cout << aR << endl;
        boundVarsToPred.clear();
        boundVariables.clear();

        // 2) generate the corresponding magic rules
        generate(aR,nullptr,permissibles);
        }

    // adorn the rules
    while( ! adornedPredicatesToBeProcessed.empty() )
        {
        ADORNED_PREDICATE aP = adornedPredicatesToBeProcessed.back();
        adornedPredicatesToBeProcessed.pop_back();

        auto targetIt = adornedPredicatesTargets.find(aP);
        assert( targetIt != adornedPredicatesTargets.end() );
        TARGET target = targetIt->second;
        targetIt->second = target == NORMAL_RULES ? NORMAL_RULES_DONE : ALL_RULES_DONE;

        trace_msg(grounding,2,"*** Adorning Rules w.r.t. Predicate: " << aP
                   << " and target " << targetToString( target ) << endl);

        PREDICATES_RULES_INDEX::const_iterator aPrules = predicatesRulesIndex.find(aP.getPredicate());

        if( aPrules == predicatesRulesIndex.end() )
            {
            // No rules for this predicate!
            continue;
            }

        for( vector<size_t>::const_iterator ruleIt = aPrules->second.begin();
             ruleIt != aPrules->second.end();
             ruleIt++ )
            {
            Rule *rule = IDB[*ruleIt];
            assert( rule != nullptr );
///////////////// TODO: chiedi a Jessica se il vecchio rule.isDisjunctive() può essere equivalente a rule.getSizeHead() > 1
            if( rule->getSizeHead() > 1 ? target == NORMAL_RULES : target == DISJUNCTIVE_RULES )
                {
                trace_msg(grounding,2,"Skip " << rule << endl);
                continue;
                }

            const vector<Atom*> &head = rule->getHead();

            for( vector<Atom*>::const_iterator headAtomIt = head.begin();
                 headAtomIt != head.end();
                 headAtomIt++ )
                {
                assert( (*headAtomIt)->hasPredicate() && aP.getPredicate() != nullptr );
                // if the current name atom, *headAtomIt, matches with the adorning
                // predicate name (unadorned), then
                // 1) build the rule and adorn it,
                // 2) generate the corresponding magic rules,
                // 3) modify the adorned rule.
                if( aP.getPredicate()->getIndex() != (*headAtomIt)->getPredicate()->getIndex() )
                    continue;

                // 1) build the rule and adorn it
                ADORNED_RULE aR;
                int permissibles = adornRule(
                        rule,
                        new ADORNED_ATOM(aP,(*headAtomIt)->getTerms(),(*headAtomIt)->isHasMinus(),(*headAtomIt)->isNegative()),
                        target == NORMAL_RULES,
                        aR
                );

                // 2) generate the corresponding magic rules
                generate(aR, *aR.getHead().begin(), permissibles);

                // 3) modify the adorned rule
                modify(*rule, aR);
                }
            }
        }
    trace_action(grounding,1,{cerr << endl << endl << " Magic Rules: " << magicRules.size() << endl; for( auto r: magicRules ) cerr << *r << endl;});
    trace_action(grounding,1,{cerr << endl << endl << " Modified Rules: " << modifiedRules.size() << endl; for( auto r: modifiedRules ) cerr << *r << endl;});

//cerr << "IDB old size: " << IDB.size() << endl;
//cerr << "Rewritten program: " << magicRules.size()+modifiedRules.size() << endl;
//
//std::clock_t start;
//double duration;
//start = std::clock();

    manageLostBindings();

//duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
//std::cerr<<"Manage Lost binding opt duration (sec): "<< duration << endl;

    // REWRITING THE ORIGINAL RULES
    // substitute the rewritten rules for the original ones
    for( auto r: IDB )
        {
        r->free();
        delete r;
        }
    IDB.erase( IDB.begin(), IDB.end() );
    IDB.insert(IDB.begin(), queryRules.begin(),queryRules.end());
    for( auto mr: magicRules )
        {
        IDB.push_back(mr->getRule());
        delete mr;
        }
    for( auto mr: modifiedRules )
        {
        IDB.push_back(mr->getRule());
        delete mr;
        }

    // print the magic program
    trace_action(grounding,1,{
        cerr << endl << endl << " Rewritten program" << endl;
        for( auto r: IDB ) r->print(cerr); cerr << endl;
        cerr << endl << "( Totally: " << IDB.size() << " rewritten rules )\n";
        cerr << endl <<endl;
        });

//    // check subsumption
//    if( OptionSubsumptionCheckingAfterDMS ||
//            OptionOptimizedDisjunctiveMagicSets )
//        {
//        if( TraceLevel >= 1 )
//            {
//            cdebug << endl << endl
//                   << " Subsumption checking of rewritten"
//                   << " and unadorned IDB rules will be performed" << endl;
//            }
//
//        SubsumptionCheckingRewriting( IDB, firstRule );
//        }
    }

bool
MAGIC_SETS::processQuery()
    {
    bool rewrittenQuery = false;
    if( ! IsQueryGround )
        {
        PREDICATES_RULES_INDEX::const_iterator qr = predicatesRulesIndex.find(PredicateTable::getInstance()->getQueryPredicate());

        assert( qr != predicatesRulesIndex.end() );
        assert( qr->second.size() == 1 );

        Rule *queryRule = IDB[qr->second.front()]->clone();
        const vector<Atom*> &head = queryRule->getHead();

        assert( head.size() == 1 );
        assert( (*head.begin())->hasPredicate() );
        assert( ! (*head.begin())->getPredicate()->isEdb() );
        assert( (*head.begin())->getTermsSize() != 0 );

//        modifiedRules.push_back(queryRule);
        queryRules.push_back(queryRule);

        ABSTRACT_ADORNED_ATOM *aa = new ADORNED_ATOM(*(head.begin()));

        assert( aa->isFullyFree() );
        ADORNED_RULE adornedRule;
        unsigned permissibles = adornRule(queryRule,aa,false,adornedRule);

        // if there is at least one literal in the query rule
        // that can propagate a binding (i.e. a literal with
        // some bound argument), generate the magic rules and
        // adorn the program.
/////////////// TODO: probabilmente vogliamo eseguirli in ogni caso, ma controlla se è così per sicurezza
//        if( OptionMagicSetsExplicit || adornedRule.hasBoundLiterals() )
            {
            generate(adornedRule,nullptr,permissibles);
            rewrittenQuery = true;
            }
//        else
//            {
//            rewrittenQuery = false;
//            if( TraceLevel >= 2 )
//                cdebug << "The query is completely non-ground." << endl;
//            }
        }
    else
        {
        // If the query is completely ground it is always possible to
        // adorn it.
        ADORNED_RULE adornedRule;
        for( vector<Atom*>::const_iterator i = Query->begin();
             i != Query->end();
             i++ )
            {
            if( ! (*i)->isClassicalLiteral() || (*i)->getPredicate()->isEdb() )
                continue;

            ABSTRACT_ADORNED_ATOM *aa = new ADORNED_ATOM(*i);
            insertPredicate(aa->getAdornedPredicate(),false);
            adornedRule.getBody().push_back(aa);
            }

        generate(adornedRule,nullptr,Query->size());
        rewrittenQuery = true;
        }

    return rewrittenQuery;
    }

int MAGIC_SETS::adornRule(
    Rule *r,
    ABSTRACT_ADORNED_ATOM *aA,
    bool originatedFromDisjunctiveRule,
    ADORNED_RULE &outRule )
    {
	assert( aA != nullptr );
    assert( r != nullptr );
    assert( boundVarsToPred.empty() );
    assert( boundVariables.empty() );
    assert( r->getSizeHead() > 0 );

    trace_action(grounding,2,cerr << "Adorning   "; r->print(cerr); cerr << endl;);

    aA->addBoundVariables( boundVariables, boundVarsToPred, true );
    outRule.getHead().push_back(aA);

    int countPermissible = 0;
    if( r->getSizeBody() > 0 )
        countPermissible = adornBody(r->getBody(),originatedFromDisjunctiveRule,outRule);

    // and this is also the case for all head atoms different from aA
    for( vector<Atom*>::const_iterator it = r->getHead().begin();
            it != r->getHead().end();
            ++it )
        {
        if( ! (*(*it) == *(aA->getAtom())) )
            adornAndAddAtomToHead(*it,originatedFromDisjunctiveRule,outRule);
        }

    trace_msg(grounding,2,"Adorned rule:   " << outRule << endl);

    boundVarsToPred.clear();
    boundVariables.clear();

    return countPermissible;
    }

int
MAGIC_SETS::adornBody(
    const std::vector<Atom*> &conj,
    bool originatedFromDisjunctiveRule,
    ADORNED_RULE &outRule )
    {
    int countPermissible = 0;

    // store all the body literals of r in notYetConsideredLiterals
    vector<Atom*> notYetConsideredLiterals;
    notYetConsideredLiterals.insert(
            notYetConsideredLiterals.begin(),
            conj.begin(),
            conj.end() );

    // add the permissible literals of the original body to the
    // new body according to the order defined by the preferential
    // criterion.
    while( ! notYetConsideredLiterals.empty() )
        {
        vector<Atom*>::iterator i = notYetConsideredLiterals.begin();
        // selection of the first permissible LITERAL to propagate bindings
        while( i != notYetConsideredLiterals.end()  )
            {
            if( isPermissible(*i) )
                // first permissible literal found
                break;

            // not yet found a permissible literal
            ++i;
            }

        if( i == notYetConsideredLiterals.end() )
            break;
        // selection of the best permissible LITERAL to
        // propagate bindings, through the comparison of the
        // chosen permissible body literal with the remaining
        // permissible body literals
        vector<Atom*>::iterator j = i + 1;
        while( j != notYetConsideredLiterals.end() )
            {
            if( isPermissible(*j) )
                {
                // another permissible literal found;
                // choose the preferable one
                if( ! isPreferable(*i,*j) )
                    i = j;
                }
            ++j;
            }

        adornAndAddLiteralToBody(*i,originatedFromDisjunctiveRule,outRule);

        // update the set of bound variables of the current
        // rule adding all of the variables of the just chosen
        // literal
        //if( hasBoundTerms( *i ) || ( i->isEDB() && ! i->isPropositional() ) )
        if( ! (*i)->getTermsSize() == 0 && hasBoundTerms( *i ) )
            updateBoundVars(*i);

        // we have considered the literal *i
///////////////////// MARIO
//        *i = notYetConsideredLiterals.back();
//        notYetConsideredLiterals.pop_back();
///////////////////// END

///////////////////// EX DLV
        notYetConsideredLiterals.erase(i);
///////////////////// END

        countPermissible++;
        }

    // if there are remaining literals it means that they are not
    // permissible and we do not have to propagate the adornment
    // through them, but they must be added to the body of the new
    // rule in any case
    for( vector<Atom*>::iterator k = notYetConsideredLiterals.begin();
         k != notYetConsideredLiterals.end();
         k++ )
        adornAndAddLiteralToBody( *k, originatedFromDisjunctiveRule, outRule );

    return countPermissible;
    }

void
MAGIC_SETS::adornAndAddAtomToHead(
    Atom *a,
    bool originatedFromDisjunctiveRule,
    ADORNED_RULE &r )
    {
    assert( a != nullptr && a->hasPredicate() && ! a->getPredicate()->isEdb() );
    ABSTRACT_ADORNED_ATOM *aA = new ADORNED_ATOM(a,boundVariables,boundVarsToPred,statementAtomMapping);
    insertPredicate(aA->getAdornedPredicate(),originatedFromDisjunctiveRule || r.isDisjunctive());
    r.getHead().push_back( aA );
    }

void
MAGIC_SETS::adornAndAddLiteralToBody(
    Atom *L,
    bool originatedFromDisjunctiveRule,
    ADORNED_RULE &r )
    {
    assert( L != nullptr );
    ABSTRACT_ADORNED_ATOM *aL = nullptr;
    if( L->isAggregateAtom() )
    	{
    	aL = new AGGREGATE_ADORNED_ATOM(L,boundVariables,boundVarsToPred,statementAtomMapping);
    	for( unsigned i=0; i<L->getAggregateElementsSize(); i++ )
			{
    		Atom *tmp = L->getAggregateElement(i)->getNafLiteral(0);
    		if( tmp->hasPredicate() && ! tmp->getPredicate()->isEdb() )
    			insertPredicate(aL->getAdornedPredicate(i),originatedFromDisjunctiveRule);
			}
    	}
    else
    	{
		aL = new ADORNED_ATOM(L,boundVariables,boundVarsToPred,statementAtomMapping);
		if( L->hasPredicate() && ! L->getPredicate()->isEdb() )
			insertPredicate(aL->getAdornedPredicate(),originatedFromDisjunctiveRule);
    	}
	r.getBody().push_back(aL);
    }

bool
MAGIC_SETS::isPermissible(
    const Atom *L )
    {
    assert( L != nullptr );
    if ( L->getTermsSize() == 0 /*|| (L->hasPredicate() && L->getPredicate()->isEdb())*/ )
        return true;
    if( L->isAggregateAtom() )
    	return false;
    if( (L->isNegative() || (L->isBuiltIn() && L->getBinop()!=EQUAL)) && fullyBound(L) )
        return true;

    // FIXME: binding through builtins disabled for the moment.
    // if ( L.isBuiltin() && ( L.getBuiltinID()==BUILTIN_TYPE::BuiltinAddition
    //                         || L.getBuiltinID()==BUILTIN_TYPE::BuiltinMultiplication )
    //                    && hasBoundTerms(L,BV) )
    //     return true;
    if ( !L->isNegative() && !L->isBuiltIn() )
        return true;

    return false;
    }

bool
MAGIC_SETS::fullyBound(
    const Atom *L )
    {
    assert( L != nullptr );
    assert( L->getTermsSize() != 0 );
    const vector<Term*> &params = L->getTerms();
    for( vector<Term*>::const_iterator i=params.begin();
         i != params.end();
         i++ )
        {
        switch( (*i)->getType() )
            {
        case TermType::ANONYMOUS:
            return false;

        case TermType::VARIABLE:
            if( boundVariables.find(*i) == boundVariables.end() )
                return false;
            else
                {
                Predicate* pL = L->getPredicate();
                Predicate* pB = nullptr;
                auto it = boundVarsToPred.find(*i);
                if( it != boundVarsToPred.end() )
                    {
                    pB = it->second;
                    if( pL != nullptr && pB != nullptr && statementAtomMapping.checkIfDepended(pB,pL) )
                        return false;
                    }
                }
            break;

//////////////// MARIO
//        case TermType::FUNCTION:
//            {
//            set_term vars;
//            (*i)->getVariable(vars);
//            bool allBound = true;
//            for( auto v:vars )
//                if( boundVariables.find(v) == boundVariables.end() ) {
//                    allBound = false;
//                    break;
//                }
//            if( ! allBound )
//                return false;
//            break;
//            }
//////////////// END

        default:
            break;
            }
        }

    return true;
    }

unsigned
MAGIC_SETS::countNumberBoundTerms(
    const Atom *a )
    {
    assert( a != nullptr );
    if( a->getTermsSize() == 0 )
        return 0;

    unsigned count = 0;
    const vector<Term*> &params = a->getTerms();
    for( vector<Term*>::const_iterator i = params.begin();
         i != params.end();
         i++ )
        {
        if( (*i)->isGround() )
            count++;

        else if( (*i)->getType()==VARIABLE )
            {
            auto it = boundVariables.find(*i);
            if( it != boundVariables.end() )
                {
                Predicate* pA = a->getPredicate();
                Predicate* pB = nullptr;
                auto it = boundVarsToPred.find(*i);
                if( it != boundVarsToPred.end() )
                    {
                    pB = it->second;
                    if( pA != nullptr && pB != nullptr && statementAtomMapping.checkIfDepended(pB,pA) )
                        continue;
                    }
                count++;
                }
            }
///////////////// MARIO
//        switch( (*i)->getType() )
//            {
//        case TermType::VARIABLE:
//            if( boundVariables.find(*i) != boundVariables.end() )
//                count++;
//            break;
//
//        case TermType::NUMERIC_CONSTANT:
//        case TermType::STRING_CONSTANT:
//        case TermType::SYMBOLIC_CONSTANT:
//            count++;
//            break;
//
//        case TermType::FUNCTION:
//            {
//            set_term vars;
//            (*i)->getVariable(vars);
//            bool allBound = true;
//            for( auto v:vars )
//                if( boundVariables.find(v) == boundVariables.end() ) {
//                    allBound = false;
//                    break;
//                }
//            if( allBound )
//                count++;
//            break;
//            }
//
//        default:
//            break;
//            }
//        }
///////////////// END
        }
    return count;
    }

bool
MAGIC_SETS::hasBoundTerms(
    const Atom *a ) const
    {
    assert( a != nullptr );
    if( a->getTermsSize() == 0 )
        return false;

    const vector<Term*> &params = a->getTerms();
    for( vector<Term*>::const_iterator i = params.begin();
         i != params.end();
         i++ )
        {
        if( ( (*i)->getType()==VARIABLE   && boundVariables.find(*i) != boundVariables.end()) || (*i)->isGround() )
            return true;
        }
    return false;

/////////////// MARIO
//        switch( (*i)->getType() )
//            {
//        case TermType::VARIABLE:
//            if( boundVariables.find(*i) != boundVariables.end() )
//                return true;
//            break;
//
//        case TermType::FUNCTION:
//            {
//            set_term vars;
//            (*i)->getVariable(vars);
//            bool allBound = true;
//            for( auto v:vars )
//                if( boundVariables.find(v) == boundVariables.end() ) {
//                    allBound = false;
//                    break;
//                }
//            if( allBound )
//                return true;
//            break;
//            }
//
//        case TermType::STRING_CONSTANT:
//        case TermType::NUMERIC_CONSTANT:
//        case TermType::SYMBOLIC_CONSTANT:
//            return true;
//
//        default:
//            break;
//            }
//        if( (*i)->isGround() )
//            return true;
//        }
//    return false;
/////////////// END
    }

bool
MAGIC_SETS::isPreferable(
    const Atom *L1,
    const Atom *L2 )
    {
    assert( L1 != nullptr && L2 != nullptr );
    // an undefined propositional literal is always preferable
//////////////////////// TODO: come si potrebbe tradurre questa isUndef? Quando l'atomo è proposizionale e undefined sarebbe preferibile
    // poiché se dovesse essere falso questo sega una parte di riscrittura.
//    if( L1->getTermsSize() == 0 && L1->isUndef() )
//        return true;
    // an EDB propositional literal is always preferable
    if( L1->hasPredicate() && L1->getPredicate()->isEdb() && L1->getTermsSize() == 0 )
        return true;
    if( L2->hasPredicate() && L2->getPredicate()->isEdb() && L2->getTermsSize() == 0 )
        return false;
    // an IDB propositional literal is never preferable
    if( L1->hasPredicate() && ! L1->getPredicate()->isEdb() && L1->getTermsSize() == 0 )
        return false;
    if( L2->hasPredicate() && ! L2->getPredicate()->isEdb() && L2->getTermsSize() == 0 )
        return true;

    unsigned boundVariablesInL1 = countNumberBoundTerms(L1);
    unsigned boundVariablesInL2 = countNumberBoundTerms(L2);

// Fully bound atoms should preferable, especially in case of built-ins.
// Uncomment the following lines of code to privilege fully bound atoms.
//    if( boundVariablesInL1 == L1->getTermsSize() )
//        return true;
//    if( boundVariablesInL2 == L2->getTermsSize() )
//        return false;

    // if the first literal has bound terms and the second has not, it
    // is preferable
    if( boundVariablesInL1 && ! boundVariablesInL2 )
        return true;
    // if both literals have bound terms...
    if( boundVariablesInL1 && boundVariablesInL2 )
        {
        // ... an EDB or a builtin is preferable
        if( (L1->hasPredicate() && L1->getPredicate()->isEdb()) || L1->isBuiltIn() )
            return true;
        assert( L1->hasPredicate() && ! L1->getPredicate()->isEdb() );

        if( ! L2->hasPredicate() || ! L2->getPredicate()->isEdb() )
            // ... and if both literals are IDB, the one with
            // fully bound is preferable
            {
            if( boundVariablesInL1 == L1->getTermsSize() )
                return true;
            if( boundVariablesInL2 == L2->getTermsSize() )
                return false;
            // if both are not fully bound, the one with
            // the greater number of bound variables is
            // preferable
            return boundVariablesInL1 >= boundVariablesInL2;
            }
        }

    // if both literals are completely free, choose the one which is
    // EDB or a builtin; if even this does not decide, try to keep the
    // original ordering
    if( ! boundVariablesInL1 && ! boundVariablesInL2
        && ( (L1->hasPredicate() && L1->getPredicate()->isEdb()) || L1->isBuiltIn() || (! L2->getPredicate()->isEdb()) ) )
        return true;
    return false;
    }

void
MAGIC_SETS::updateBoundVars(
    const Atom *L )
    {
    assert( L != nullptr && L->getTermsSize() > 0 );
    const vector<Term*> &params = L->getTerms();

    for( vector<Term*>::const_iterator i = params.begin();
         i != params.end();
         i++ )
        {
        switch( (*i)->getType() )
            {
        case TermType::VARIABLE:
            if( boundVariables.find(*i) == boundVariables.end() )
                // Why do not we check whether the binding comes from the head? Because updates come from body atoms only.
                boundVarsToPred.insert(pair<Term*,Predicate*>(*i,L->getPredicate()));
            boundVariables.insert(*i);
            break;

///////////// MARIO
//        case TermType::FUNCTION:
//            (*i)->getVariable(boundVariables);
//            break;
///////////// END

        default:
            break;
            }
        }
    }

void
MAGIC_SETS::insertPredicate(
    const ADORNED_PREDICATE &aP,
    bool fromDisjunctiveRule )
    {
    auto targetIt = adornedPredicatesTargets.find(aP);
    if( targetIt != adornedPredicatesTargets.end() )
        {
        if( fromDisjunctiveRule )
            {
            trace_msg(grounding,2,"Predicate " << aP << " (generated by a disjunctive rule) is already in table with target "
                       << targetToString(targetIt->second) << "." << endl);
            return;
            }

        TARGET &target = targetIt->second;
        switch( target )
            {
        case NORMAL_RULES:
            target = ALL_RULES;
            trace_msg(grounding,2,"Predicate " << aP << " changed target from NORMAL_RULES to ALL_RULES." << endl);
            return;

        case NORMAL_RULES_DONE:
            target = DISJUNCTIVE_RULES;
            break;

        default:
            trace_msg(grounding,2,"Predicate " << aP << " is already in table with target "
                       << targetToString( targetIt->second ) << "." << endl);
            return;
            }
        }
    else
        adornedPredicatesTargets.insert(
                unordered_map<ADORNED_PREDICATE,TARGET,ADORNED_PREDICATE::hasher>::value_type(
                        aP, fromDisjunctiveRule ? NORMAL_RULES : ALL_RULES
                )
        );

    adornedPredicatesToBeProcessed.push_back(aP);
    trace_msg(grounding,2,"Predicate " << aP << " inserted in table with target "
               << targetToString(adornedPredicatesTargets[aP]) << "."
               << endl);
    }

void
MAGIC_SETS::generate(
    const ADORNED_RULE &aR,
    ABSTRACT_ADORNED_ATOM *sourceAtom,
    int permissibles )
    {
    assert( boundVarsToPred.empty() );
    assert( boundVariables.empty() );
    trace_msg(grounding,2,"****GENERATE****" << endl);

    assert( sourceAtom == nullptr || aR.getHead().size() >= 1 );
    assert( sourceAtom == nullptr || *sourceAtom == *(*aR.getHead().begin()) );
    assert( sourceAtom == nullptr
            || (sourceAtom->getAtom() != nullptr
                && sourceAtom->getAtom()->hasPredicate()
                && ! sourceAtom->getAtom()->getPredicate()->isEdb()) );

    vector<Atom*> baseBody;
    vector<ADORNMENT*> baseBodyAdornment;
    if( sourceAtom != nullptr )
        {
        Atom *magicAtom = magic(sourceAtom);
        ADORNMENT *magicAtomAdornment = new ADORNMENT(sourceAtom->getAdornment());
        baseBody.push_back(magicAtom);
        baseBodyAdornment.push_back(magicAtomAdornment);

        for( ADORNED_RULE::HEAD::const_iterator i = aR.getHead().begin()+1;
             i != aR.getHead().end();
             i++ )
            {
            assert( (*i)->getAtom() != nullptr && (*i)->getAtom()->hasPredicate() && ! (*i)->getAtom()->getPredicate()->isEdb() );
            generate(aR,baseBody,baseBodyAdornment,*i,aR.getBody().end(),permissibles);
            }
        }

    // When a magic rule has to be generated for a literal p, it must
    // include in the body of the magic rule only the literals
    // useful for propagating bindings from the head of the rule to p.
    // Any useless literal (EDB or IDB) is included only if it is
    // propositional or undefined.
    // for each adorned predicate of the body with bound
    // arguments build a magic rule
    for( ADORNED_RULE::BODY::const_iterator i = aR.getBody().begin();
         i != aR.getBody().end();
         i++ )
        {
        if( (*i)->getAtom() != nullptr && (*i)->getAtom()->hasPredicate() && ! (*i)->getAtom()->getPredicate()->isEdb() )
            generate(aR,baseBody,baseBodyAdornment,(*i),i, permissibles);
        else if( (*i)->getAtom() != nullptr && (*i)->getAtom()->isAggregateAtom() )
        	{
        	for( unsigned j=0; j<(*i)->getAtom()->getAggregateElementsSize(); j++ )
				{
        		Atom *tmp = (*i)->getAtom()->getAggregateElement(j)->getNafLiteral(0);
                if( tmp != nullptr && tmp->hasPredicate() && ! tmp->getPredicate()->isEdb() )
                	{
                	ABSTRACT_ADORNED_ATOM *adornedTmp = new
                			ADORNED_ATOM(tmp,(*i)->getAdornment(j));
                    generate(aR,baseBody,baseBodyAdornment,adornedTmp,i, permissibles);
                    delete adornedTmp;
                	}
				}
        	}
        }

    for( auto a: baseBody )
        delete a;
    for( auto a: baseBodyAdornment )
        delete a;
    assert( boundVarsToPred.empty() );
    assert( boundVariables.empty() );
    }

void
MAGIC_SETS::generate(
    const ADORNED_RULE &aR,
    vector<Atom*> partOfNewMagicRuleBody,
    vector<ADORNMENT*> partOfNewMagicRuleBodyAdornment,
    ABSTRACT_ADORNED_ATOM *headAtom,
    ADORNED_RULE::BODY::const_iterator end,
    int permissibles )
    {
    assert( partOfNewMagicRuleBody.size() <= 1 );
    assert( partOfNewMagicRuleBody.size() == partOfNewMagicRuleBodyAdornment.size() );

    // the new rule must have in the head the magic
    // version of the current body literal ...
    Atom* magicI = magic(headAtom);
    // to avoid generating a useless "reflexive"
    // magic rule of the form
    // "magic#p#bf(V):- a(Y,V),magic#p#bf(V)", it is
    // necessary to check if the literal of the
    // body to be "switched" with the head of the
    // adorned rule is not equal.
    assert( magicI != nullptr &&
            (partOfNewMagicRuleBody.empty() || *partOfNewMagicRuleBody.begin() != nullptr) &&
            (partOfNewMagicRuleBodyAdornment.empty() || *partOfNewMagicRuleBodyAdornment.begin() != nullptr));
    if( partOfNewMagicRuleBody.size() == 1 && (*magicI) == (*(*partOfNewMagicRuleBody.begin())) )
    {
        delete magicI;
        return;
    }
    // build the new magic rule
    DECORATED_RULE *decoratedMagicRule = new DECORATED_RULE();
    decoratedMagicRule->setRule(new Rule());

    if( partOfNewMagicRuleBody.size() == 1 )
        {
        decoratedMagicRule->pushToBody((*partOfNewMagicRuleBody.begin())->clone(),*partOfNewMagicRuleBodyAdornment.begin());
        trace_msg(grounding,2," Add dependency : " << MAGIC_SETS::getMagicRep(headAtom->getAtom()->getPredicate())->getName() << " -> " << MAGIC_SETS::getMagicRep((*partOfNewMagicRuleBody.begin())->getPredicate())->getName() << endl);
        statementAtomMapping.addPredicateDependencies(MAGIC_SETS::getMagicRep(headAtom->getAtom()->getPredicate()), MAGIC_SETS::getMagicRep((*partOfNewMagicRuleBody.begin())->getPredicate()));
        }
    const ADORNMENT *ad = &(headAtom->getAdornment());
    magicI->setNegative(false);
    decoratedMagicRule->pushToHead(magicI,ad);

    trace_msg(grounding,2,"Generating magic rule w.r.t literal: " << headAtom->toString() << endl);

    assert( boundVarsToPred.empty() );
    assert( boundVariables.empty() );
    headAtom->addBoundVariables(boundVariables,boundVarsToPred,true);

    // add to the new magic rule all the literals
    // useful to propagate binding to the considered
    // body literal *i and preceding it in the body
    // NOTE: Literals are processed in reverse order.
    if( end > aR.getBody().begin() )
        {
        vector< ADORNED_RULE::BODY::const_iterator > builtinsAndNegativeAtomsIt;
        ADORNED_RULE::BODY::const_iterator j = end;
        do
            {
            --j;

            assert( (*j)->getAtom() != nullptr &&
                    ((*j)->getAtom()->isBuiltIn() || (*j)->getAtom()->isAggregateAtom() ||
                    ((*j)->getAtom()->hasPredicate() && (*j)->getAtom()->getPredicate()->isEdb() ) ||
                    ((*j)->getAtom()->hasPredicate() && ! (*j)->getAtom()->getPredicate()->isEdb() ) ) );

            trace_msg(grounding,2," checking utility of literal " << (*j)->toString() << "..." << endl);

            if( (*j)->getAtom()->isAggregateAtom() )
				{
				trace_msg(grounding,2,"  aggregate atoms are not added to magic rules." << endl);
				continue;
				}
            if( (*j)->getAtom()->hasPredicate() && headAtom->getAtom()->hasPredicate() && statementAtomMapping.checkIfDepended((*j)->getAtom()->getPredicate(), headAtom->getAtom()->getPredicate()) )
                {
                trace_msg(grounding,2,"" << ((*j)->getAtom()->getPredicate())->getName() << " is dependant from the head predicate " << headAtom->getAtom()->getPredicate()->getName() << endl);
                continue;
                }

            // EDB propositional literal or undefined
            // propositional literal are considered
            // useful to be inserted in the magic rule,
            // even if they are not properly useful to
            // propagate any binding.
            if( (*j)->getAtom() != nullptr && (*j)->getAtom()->getTermsSize() == 0 )
                {
                if( (*j)->getAtom()->hasPredicate() && (*j)->getAtom()->getPredicate()->isEdb() )
//                if( j->isUndef() || j->isEDB() )
                    {
                    decoratedMagicRule->pushToBody((*j)->getAtom()->clone());
                    trace_msg(grounding,2," Add dependency : " << MAGIC_SETS::getMagicRep(headAtom->getAtom()->getPredicate())->getName() << " -> " << (*j)->getAtom()->getPredicate()->getName() << endl);
                    statementAtomMapping.addPredicateDependencies(MAGIC_SETS::getMagicRep(headAtom->getAtom()->getPredicate()), (*j)->getAtom()->getPredicate());
                    }
                else
                    trace_msg(grounding,2,"  not useful." << endl);
                continue;
                }

//            set_term::const_iterator k;

//////////////////// MARIO
//            if( j - aR.getBody().begin() < permissibles && isUseful(*j) )
//                {
//                if( j->getAtom() != nullptr && j->getAtom()->hasPredicate() && ! j->getAtom()->getPredicate()->isEdb() )
//                    j->addBoundVariables( boundVariables, boundVarsToPred );
//                else
//                    updateBoundVars(j->getAtom());
//                decoratedMagicRule->pushToBody(j->getAtom()->clone());
//                }
//            else
//                trace_msg(grounding,2,"  not useful." << endl);
//////////////////// END

//////////////////// EX DLV
            bool useful = false;
            const vector<Term*> &jTerms = (*j)->getAtom()->getTerms();
            for( auto k = boundVariables.begin();
                 k != boundVariables.end() && !useful;
                 k++)
                {
                vector<Term*>::const_iterator it = find(jTerms.begin(),jTerms.end(),*k);
                size_t pos = it - jTerms.begin();

                if( ((*j)->getAtom()->hasPredicate() && (*j)->getAtom()->getPredicate()->isEdb()) && find(jTerms.begin(),jTerms.end(),*k) != jTerms.end() )
                    {
                    useful=true;
                    updateBoundVars((*j)->getAtom());
                    }
                else if( (*j)->getAtom()->isBuiltIn() && find(jTerms.begin(),jTerms.end(),*k) != jTerms.end() )
                    {
                    builtinsAndNegativeAtomsIt.push_back(j);
                    }
                else
                    {
                    assert( (*j)->getAtom()->hasPredicate() );
                    if( isPermissible((*j)->getAtom()) &&
                          it != jTerms.end() &&
						  (*j)->getAdornment()[pos].isFullyFree() )
                        {
                        // j may have a full free adornment in position pos because it could not receive the binding from the predicate that
                        // made the variable bound for the first time, so check whether it is actually the source of the current binding;
                        bool isBindingSource = true;
                        ADORNED_RULE::BODY::const_iterator myIt = j;
                        --myIt;
                        while( myIt > aR.getBody().begin() && isBindingSource ) {
                            if( (*myIt)->getAtom() != nullptr &&
								(*myIt)->getAtom()->getTermsSize() != 0 &&
                                find((*myIt)->getAtom()->getTerms().begin(),(*myIt)->getAtom()->getTerms().end(),*k) != (*myIt)->getAtom()->getTerms().end() )
                                isBindingSource = false;
                            --myIt;
                        }

                        if( isBindingSource )
                            {
                            if( (*j)->getAtom()->isNegative() && (*j)->getAtom()->getTermsSize() > 0  )
                                {
                                builtinsAndNegativeAtomsIt.push_back(j);
                                break;
                                }
                            useful =true;
                            boundVariables.erase(find(boundVariables.begin(),boundVariables.end(),*k));
                            if( (*j)->getAtom()->isBuiltIn() )
                                updateBoundVars((*j)->getAtom());
                            else
                            	(*j)->addBoundVariables(boundVariables,boundVarsToPred);
                            }
                        break;
                        }
                    }
                }
            if( useful )
                {
                decoratedMagicRule->pushToBody((*j)->getAtom()->clone());
                if( (*j)->getAtom()->hasPredicate() )
                    {
                    trace_msg(grounding,2," Add dependency : " << MAGIC_SETS::getMagicRep(headAtom->getAtom()->getPredicate())->getName() << " -> " << (*j)->getAtom()->getPredicate()->getName() << endl);
                    statementAtomMapping.addPredicateDependencies(MAGIC_SETS::getMagicRep(headAtom->getAtom()->getPredicate()), (*j)->getAtom()->getPredicate());
                    }
                }
            else
                trace_msg(grounding,2,"  not useful." << endl);
//////////////////// END
            }
        while( j > aR.getBody().begin() );

        if( builtinsAndNegativeAtomsIt.size() > 0 )
            {
            set_term ruleTerms;
            for( auto bodyAtomIt : decoratedMagicRule->getBody() )
                for( auto termIt : bodyAtomIt->getTerms() )
                    ruleTerms.insert(termIt);
            for( auto atomIt : builtinsAndNegativeAtomsIt )
                {
                bool safe = true;
                for( auto termIt : (*atomIt)->getAtom()->getTerms() )
                    if( ruleTerms.find(termIt) == ruleTerms.end() )
                        safe = false;
                if( safe )
                    {
                    decoratedMagicRule->pushToBody((*atomIt)->getAtom()->clone());
                    if( (*atomIt)->getAtom()->hasPredicate() && statementAtomMapping.checkIfDepended((*atomIt)->getAtom()->getPredicate(), headAtom->getAtom()->getPredicate()) )
                        {
                        trace_msg(grounding,2," Add dependency : " << MAGIC_SETS::getMagicRep(headAtom->getAtom()->getPredicate())->getName() << " -> " << (*atomIt)->getAtom()->getPredicate()->getName() << endl);
                        statementAtomMapping.addPredicateDependencies(MAGIC_SETS::getMagicRep(headAtom->getAtom()->getPredicate()), (*atomIt)->getAtom()->getPredicate());
                        }
                    }
                }
            }
        }

    trace_action(grounding,2,{
            cerr << "Generated magic rule\n   "
              << *decoratedMagicRule
              << endl;});
//////////////////////////// TODO: verifica se l'opzione OptionOptimizedDisjunctiveMagicSets è ancora necessaria!
//////////////////////////// Il check di sussunzione, invece, è fatto alla fine.
//    if( ! OptionOptimizedDisjunctiveMagicSets ||
//            ! subsumption(magicRule, magicRules ) )

        magicRules.push_back(decoratedMagicRule);

        trace_msg(grounding,2," Rule added to MagicRules." << endl);
//        }

    boundVarsToPred.clear();
    boundVariables.clear();
    }

bool
MAGIC_SETS::isUseful(
    const ADORNED_ATOM &aL )
    {
    assert( aL.getAtom() != nullptr );
    const vector<Term*> &args = aL.getAtom()->getTerms();
    for( vector<Term*>::const_iterator it = args.begin(); it != args.end(); it++ )
        {
        if( aL.getAtom()->hasPredicate() && ! aL.getAtom()->getPredicate()->isEdb() && aL.getAdornment()[it-args.begin()].isFullyBound() )
            continue;

        set_term vars;
        switch( (*it)->getType() )
            {
        case TermType::VARIABLE:
            vars.insert( *it );
            break;

//////////////// MARIO
//        case TermType::FUNCTION:
//            (*it)->getVariable(vars);
//            break;
//////////////// END

        default:
            break;
            }

        bool allBound = true;
        for( auto v:vars )
            if( boundVariables.find(v) == boundVariables.end() ) {
                allBound = false;
                break;
            }
        if( ! allBound )
            continue;

        trace_action(grounding,2, cerr << "  literal " << aL
                   << " is useful to propagate binding of term "; (*it)->print(cerr); cerr << endl;);

        for( set_term::const_iterator vIt = vars.begin(); vIt != vars.end(); vIt++ )
            boundVariables.erase( *vIt );

        return true;
        }

    return false;
    }

void
MAGIC_SETS::modify(
    const Rule &rule,
    const ADORNED_RULE &adornedRule )
    {
    trace_msg(grounding,2,"****MODIFY****\n" << "Modifying rule " << adornedRule << endl);

    const ADORNED_RULE::HEAD &head = adornedRule.getHead();
    const ADORNED_RULE::BODY &body = adornedRule.getBody();
    assert( head.size() >= 1 );
    assert( head.size() == rule.getHead().size() );
    assert( body.size() == rule.getBody().size() );

    DECORATED_RULE *decoratedModifiedRule = new DECORATED_RULE();
    decoratedModifiedRule->setRule(new Rule());

    // for each adorned literal of the head create the corresponding magic
    // literal and put it into the body of the modified rule
    for( ADORNED_RULE::HEAD::const_iterator k = head.begin();
         k != head.end();
         k++ ) {
        const ADORNMENT *adornment = &((*k)->getAdornment());
        decoratedModifiedRule->pushToBody(magic(*k),adornment);
    }

    // add also the original body literals
    for( vector<Atom*>::const_iterator it = rule.getBody().begin();
            it != rule.getBody().end();
            it++ )
        decoratedModifiedRule->pushToBody((*it)->clone());

    for( auto a: rule.getHead() )
        decoratedModifiedRule->pushToHead(a->clone());

    trace_action(grounding,2, cerr << "Modified rule "; cerr << *decoratedModifiedRule << endl;);
//////////////////////////// TODO: verifica se l'opzione OptionOptimizedDisjunctiveMagicSets è ancora necessaria!
//////////////////////////// Il check di sussunzione, invece, è fatto alla fine.
//    if( ! OptionOptimizedDisjunctiveMagicSets ||
//           ! subsumption( modifiedRule, modifiedRules ) )
//        {
        modifiedRules.push_back(decoratedModifiedRule);
//        }
    }

Atom*
MAGIC_SETS::magic(
    ABSTRACT_ADORNED_ATOM *aa )
    {
    assert( aa->getAtom() != nullptr );
    assert( aa->getAtom()->hasPredicate() && ! aa->getAtom()->getPredicate()->isEdb() );

    Atom *magicAtom = aa->createMagicAtom();
    assert( magicAtom != nullptr && magicAtom->hasPredicate() );
//    magicNames.insert(magicAtom->getPredicate());
    // Update the list of Most General Adornments (MGA) for the magified predicate.
    auto itt = predicateMostGeneralAdornments.find(aa->getAtom()->getPredicate()->getIndex());
    if( itt == predicateMostGeneralAdornments.end() )
    {
        vector<ADORNMENT*> ads;
        ads.push_back(new ADORNMENT(aa->getAdornment()));
        predicateMostGeneralAdornments[aa->getAtom()->getPredicate()->getIndex()] = ads;
    }
    else
        {
        bool isSubsumed = false;
        for( auto ad: itt->second )
            {
            assert( ad != nullptr );
            if( aa->getAdornment().isSubsumedBy(*ad) )
                {
                isSubsumed = true;
                break;
                }
            }
        if( ! isSubsumed )
            {
            // Erase any more specific adornment.
            vector<ADORNMENT*> updatedAdornments;
            for( auto ad: itt->second )
                {
                assert( ad != nullptr );
                if( ! ad->isSubsumedBy(aa->getAdornment()) )
                    updatedAdornments.push_back(ad);
                else
                    {
                    delete ad;
                    }
                }
            updatedAdornments.push_back(new ADORNMENT(aa->getAdornment()));
            predicateMostGeneralAdornments[aa->getAtom()->getPredicate()->getIndex()] = updatedAdornments;
            }
        }
    magicPredicate2OriginalPredicate[magicAtom->getPredicate()] = aa->getAtom()->getPredicate();
    return magicAtom;
    }

//void MAGIC_SETS::normalizeMagicPredicateNames()
//    {
//    for( set<string>::const_iterator it = magicPredicates.begin(); it != magicPredicates.end(); it++ )
//        {
//        // Normalize predicate names of magic predicates (of the
//        // form "magic_predicateName_adornment[_suffix]) until a really new
//        // name (not already internally defined or occurring in
//        // the input program) is found.
//
//        char str[256];
//        assert( it->c_str()[ 0 ] == '#' );
//        strcpy( str, it->c_str()+1 );
//        for(unsigned i = 0; i < strlen(str); i++)
//            {
//            if( str[i] == '#' )
//                str[i] = '_';
//            }
//
//        pair<PREDICATE_NAMES::index_t, bool> res =
//            PredicateTable::getInstance()->getPredicateName().find( str );
//        if( res.second )
//            {
//            char str2[256];
//            int count = 0;
//            do
//                {
//                sprintf( str2, "%s_%u", str, count );
//                res = ATOM::Predicates.find( str2 );
//                count++;
//                if( TraceLevel >= 3 )
//                   cdebug << "Trying to normalize " << *it << " to "
//                          << str2 << endl;
//                }
//            while(res.second);
//            strcpy( str, str2 );
//            }
//
//        if( TraceLevel >= 3 )
//           cdebug << "Normalizing " << *it
//                  << " to " << str << endl;
//
//        res = ATOM::Predicates.find( it->c_str() );
//        assert( res.second);
//        PREDICATE_NAMES::index_t idx = res.first;
//
//        // Use the result only for assertion checking at the moment.
//#ifndef NDEBUG
//        res =
//#endif
//        ATOM::Predicates.substitute(str,
//                                    ATOM::Predicates.getArity(idx),
//                                    ATOM::Predicates.getType(idx),
//                                    idx);
//
//        // Make sure we do the substitution only once; the value is
//        // false if the predicate name already existed.
//        assert( res.second );
//        }
//    }

void
MAGIC_SETS::printBoundVariables(
    ostream &out ) const
    {
    out << "Bound variables: ";
    for( set_term::const_iterator it = boundVariables.begin();
            it != boundVariables.end();
            it++ )
        {
        out << *it << " ";
        }
    out << endl;
    }

string
MAGIC_SETS::targetToString(
    TARGET target ) const
    {
    switch( target )
        {
    case ALL_RULES: return "ALL_RULES";
    case DISJUNCTIVE_RULES: return "DISJUNCTIVE_RULES";
    case NORMAL_RULES: return "NORMAL_RULES";
    case ALL_RULES_DONE: return "ALL_RULES_DONE";
    case NORMAL_RULES_DONE: return "NORMAL_RULES_DONE";
        }
    assert( 0 );
    return "ERROR";
    }

void
MAGIC_SETS::manageLostBindings()
    {
    // MANAGE CASES WHERE THE BINDING IS LOST
    // For every predicate p in IDB that is fully-free adorned:
    // - erase rules where a not fully-free adorned atom of p occurs in the body;
    // - replace every occurrence of not fully-free adorned version of p with a fully-free adorned version of p.
    trace_msg(grounding,2,"*** Manage Lost Bindings ***");
    eraseRulesWithSubsumedMagicAtomsInTheBody(modifiedRules);
    eraseRulesWithSubsumedMagicAtomsInTheBody(magicRules);
    updateHeadOfRulesWithSubsumedMagicAtoms(magicRules);
    }

void
MAGIC_SETS::eraseRulesWithSubsumedMagicAtomsInTheBody(
    vector<DECORATED_RULE*> &rules )
    {
    unsigned count = 0;
    trace_msg(grounding,2,"*** Erase rules with subsumed magic atoms ***" << endl);
    vector<DECORATED_RULE*> newRules;
    unordered_set<size_t> idxRulesToBeErased;
    for( unsigned i=0; i<rules.size(); i++ )
        {
        trace_msg(grounding,2,"Check rule " << *rules[i] << endl);
        if( rules[i]->hasBodyAnyMagicAtom() )
            {
            trace_msg(grounding,2,"Check body" << endl);
            bool toBeErased = false;
            for( unsigned j = 0; j < rules[i]->getBodySize(); j++ )
                {
                ADORNMENT *atomAdornment = rules[i]->getBodyAtomAdornment(j);
                if( atomAdornment != nullptr ) // if it is magic
                    {
                    Atom *atom = rules[i]->getBody().at(j);
                    assert( atom != nullptr && atom->hasPredicate() );
                    auto itPred = magicPredicate2OriginalPredicate.find(atom->getPredicate());
                    assert( itPred != magicPredicate2OriginalPredicate.end() );
                    assert( itPred->second != nullptr );
                    auto itt = predicateMostGeneralAdornments.find(itPred->second->getIndex());
                    assert( itt != predicateMostGeneralAdornments.end() );
                    for( auto mgaPtr: itt->second )
                        if( atomAdornment->isStrictlySubsumedBy(*mgaPtr) )
                            {
                            trace_action(grounding,2,cerr << "Atom "; atom->print(cerr); cerr << " is strictly subsumed." << endl;);
                            toBeErased = true;
                            break;
                            }
                    }
                }
            if( toBeErased )
                {
                count++;
                trace_msg(grounding,2,"Subsumed atoms: Remove!" << endl);
                rules[i]->getRule()->free();
                delete rules[i]->getRule();
                delete rules[i];
                }
            else
                {
                trace_msg(grounding,2,"No subsumed atoms in the body: Ok!" << endl);
                newRules.push_back(rules[i]);
                }
            }
        else
            {
            trace_msg(grounding,2,"No magic atoms in the body: Ok!" << endl);
            newRules.push_back(rules[i]);
            }
        }
//    cerr << "Erased rules: " << count << endl;
    rules.erase(rules.begin(),rules.end());
    rules = newRules;
    }

void
MAGIC_SETS::updateHeadOfRulesWithSubsumedMagicAtoms(
    vector<DECORATED_RULE*> &rules )
    {
    trace_msg(grounding,2,"*** Update head of rules with subsumed magic atoms ***" << endl);
    vector<DECORATED_RULE*> newRules;
    for( unsigned i=0; i<rules.size(); i++ )
        {
        trace_msg(grounding,2,"Check rule " << *rules[i] << endl);
        if( rules[i]->hasHeadAnyMagicAtom() )
            {
            bool hasBeenUpdated = false;
            // REMARK: We assume that every rule having a magic atom in the head cannot be disjunctive.
            // Otherwise, we should consider any possible combination among Most General Adornments
            // of every magic atoms.
            assert( rules[i]->getHeadSize() == 1 );
            ADORNMENT *atomAdornment = rules[i]->getHeadAtomAdornment(0);
            assert( atomAdornment != nullptr ); // It should be magic
            Atom *atom = rules[i]->getHead().at(0);
            assert( atom != nullptr && atom->hasPredicate() );
            auto itPred = magicPredicate2OriginalPredicate.find(atom->getPredicate());
            assert( itPred->second != nullptr );
            auto itt = predicateMostGeneralAdornments.find(itPred->second->getIndex());
            assert( itt != predicateMostGeneralAdornments.end() );
            for( auto mgaPtr: itt->second )
                {
                if( atomAdornment->isStrictlySubsumedBy(*mgaPtr) )
                    {
                    trace_action(grounding,2, cerr << "Predicate "; itPred->first->print(); cerr << endl;);
                    trace_action(grounding,2, cerr << " is strictly subsumed by "; mgaPtr->toString(); cerr << endl;);
                    hasBeenUpdated = true;
                    Rule *newRule = new Rule();
                    newRule->addInHead(createStrictlySubsumedAtom(atom,atomAdornment,mgaPtr));
                    newRule->setBody(rules[i]->getRule()->getClonedBody());
                    DECORATED_RULE *newDecoratedRule = new DECORATED_RULE();
                    newDecoratedRule->setRule(newRule);
                    newRules.push_back(newDecoratedRule);
                    trace_msg(grounding,2,"Subsumed magic head atoms, replace with: " << *newDecoratedRule << endl);
                    }
                }
            if( hasBeenUpdated )
                {
                rules[i]->getRule()->free();
                delete rules[i]->getRule();
                delete rules[i];
                }
            else
                newRules.push_back(rules[i]);
            }
        else
            {
            trace_msg(grounding,2,"No magic atom in the head" << endl);
            newRules.push_back(rules[i]);
            }
        }
    rules.erase(rules.begin(),rules.end());
    rules = newRules;
    }

Atom*
MAGIC_SETS::createStrictlySubsumedAtom(
    Atom *atom,
    ADORNMENT *atomAdornment,
    ADORNMENT *mga ) const
    {
    assert( atom != nullptr && atomAdornment != nullptr && mga != nullptr );
    assert( atom->hasPredicate() );
    assert( atomAdornment->getSize() == mga->getSize() );
    auto it = magicPredicate2OriginalPredicate.find(atom->getPredicate());
    assert( it != magicPredicate2OriginalPredicate.end() );

    stringstream ss;
    vector<Term*> params;
    ss << MAGIC_PREFIX << it->second->getName();

    if( atom->getTermsSize() > 0 )
        {
        ss << MAGIC_ADORNMENT_SEPARATOR << mga->toString();
        unsigned idxVar = 0;
        for( unsigned idxAd=0; idxAd<atomAdornment->getSize(); idxAd++ )
            {
            if( (*atomAdornment)[idxAd].isFullyBound() )
                {
                if( (*mga)[idxAd].isFullyBound() )
                    params.push_back(atom->getTerms().at(idxVar));
                idxVar++;
                }
            }
        }
    Predicate *predicate = new Predicate();
    predicate->setArity(params.size());
    predicate->setIdb();
    predicate->setName(ss.str());
    if(predicate->getName()!=PREDNAME_QUERY)
        predicate->setHiddenForPrinting(true);
    else
        predicate->setHiddenForPrinting(false);
    PredicateTable::getInstance()->insertPredicate(predicate);
    PredicateExtTable::getInstance()->addPredicateExt(predicate);
    return new ClassicalLiteral(predicate,params,atom->isHasMinus(),atom->isNegative());
    }

void
MAGIC_SETS::generateMagicPredsAndAddDeps()
    {
	statementAtomMapping.completeComputationOfDependencies();
	statementAtomMapping.setCompletedComputationOfDependencies(true);
	unordered_map< Predicate*, Predicate* > magicPreds;
    for( auto p = PredicateTable::getInstance()->getPredicateSetBegin();
            p != PredicateTable::getInstance()->getPredicateSetEnd();
            p++ )
        {
        Predicate* mp = MAGIC_SETS::createMagicRep(*p);
        magicPreds[*p] = mp;
        }
    for( auto it = magicPreds.begin(); it != magicPreds.end(); it++ )
        {
        PredicateTable::getInstance()->insertPredicate((*it).second);
        PredicateExtTable::getInstance()->addPredicateExt((*it).second);
        //cout << (*p)->getName() << " --> " << mp->getName() << endl;
        trace_msg(grounding,2," Add dependency : " << (*it).first->getName() << " -> " << (*it).second->getName() << endl);
        statementAtomMapping.addPredicateDependencies((*it).first,(*it).second);
        }
    statementAtomMapping.completeComputationOfDependencies();
    }


Predicate*
MAGIC_SETS::getMagicRep(
    Predicate* p )
    {
    Predicate *mp = createMagicRep(p);
    PredicateTable::getInstance()->insertPredicate(mp);
    return mp;
    }

Predicate*
MAGIC_SETS::createMagicRep(
    Predicate* p )
    {
    assert( p != NULL );

    stringstream ss;
    if( p->getName().find(MAGIC_PREFIX) == 0 )
        {
        if( p->getName().find_first_of(MAGIC_ADORNMENT_SEPARATOR) != p->getName().find_last_of(MAGIC_ADORNMENT_SEPARATOR) )
            ss << p->getName().substr( 0, p->getName().find_last_of(MAGIC_ADORNMENT_SEPARATOR) );
        else
            ss << p->getName();
        }
    else
        ss << MAGIC_PREFIX << p->getName();
    Predicate *predicate = new Predicate();
    predicate->setArity(0);
    predicate->setIdb();
    predicate->setName(ss.str());
    if(predicate->getName()!=PREDNAME_QUERY)
        predicate->setHiddenForPrinting(true);
    else
        predicate->setHiddenForPrinting(false);
    return predicate;
    }

}}
