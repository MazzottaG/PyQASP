/*
 * PropagatorWriter.h
 *
 *  Created on: Feb 8, 2017
 *      Author: jessica
 */

#ifndef SRC_GROUNDER_GROUND_PROPAGATORHEURISTICSHOLDER_H_
#define SRC_GROUNDER_GROUND_PROPAGATORHEURISTICSHOLDER_H_

#include <sstream>
#include <vector>

#include "../term/Term.h"
#include "../atom/Atom.h"
#include "../table/IdGenerator.h"
#include "../../../common/Options.h"
#include "../../util/WarningMessage.h"
#include <fstream>

using namespace std;

namespace DLV2 {
namespace grounder {

using wasp_prop = pair<Atom*,string>;

class PropagatorsHeuristicsHolder {

public:
	void setWaspOptions(){
		if(Options::mode == MODE_DLV2 && (!waspAtomsPropagator.empty() || !waspHeuristics.empty())){
			Options::interpreter=PYTHON_INTERPRETER;
			for(auto p:waspAtomsPropagator)
				Options::pluginsFilenames.push_back(p.second);
			for(auto p:waspHeuristics)
				Options::pluginsFilenames.push_back(p.second);
		}
	}

	void addWaspProp(const wasp_prop& p){
		if(Options::mode == MODE_IDLV && !waspAtomsPropagator.empty())
			WarningMessage::warningGeneric("Propagators are not supported in mode I-DLV");
		waspAtomsPropagator.push_back(p);
	}

	void addWaspHeuristics(const wasp_prop& p){
		waspHeuristics.push_back(p);
	}

	const vector<wasp_prop>& getWaspAtomsPropagator() const {
		if(Options::mode == MODE_IDLV && !waspAtomsPropagator.empty())
			WarningMessage::warningGeneric("Heuristics are not supported in mode I-DLV");
		return waspAtomsPropagator;
	}

	const vector<wasp_prop>& getWaspHeuristics() const {
		return waspHeuristics;
	}

	static PropagatorsHeuristicsHolder* getInstance(){
		if(holder==nullptr)
			holder = new PropagatorsHeuristicsHolder;
		return holder;
	}

	static void freeInstance() {
		delete holder;
	}

private:
	PropagatorsHeuristicsHolder(){};
	static PropagatorsHeuristicsHolder* holder;
	vector<wasp_prop> waspAtomsPropagator;
	vector<wasp_prop> waspHeuristics;

};

} /* namespace grounder */
} /* namespace DLV2 */

#endif /* SRC_GROUNDER_GROUND_PYTHONWASPWRITER_H_ */
