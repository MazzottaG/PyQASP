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
 * RuleStatistics.cpp
 *
 *  Created on: Mar 12, 2016
 *      Author: david
 */

#include "RuleStatistics.h"
#include "../table/PredicateExtension.h"


namespace DLV2 {
namespace grounder {

string appendSpace(string text,unsigned space){
	  if(text.size()>space){
		  text.resize(space-1);
		  return text+" ";
	  }
	  string spacePost="";
	  while(text.size()+spacePost.size()<space)spacePost+=" ";
	  return text+spacePost;
}


RuleStatistics* RuleStatistics::rstats=nullptr;

void RuleStatistics::sortVectorByTime(){
	sort(ruleStats.begin(),ruleStats.end(),[](const vector<RuleStat>& v1,const vector<RuleStat>& v2){
		double t1=0,t2=0;
		for(auto& rs:v1)
			t1+=rs.time;
		for(auto& rs:v2)
			t2+=rs.time;
		return t1>t2;
	});

}

vector<TableInfo> RuleStatistics::generateVecTableInfo() {
	vector<TableInfo> vec;
	for(unsigned index=0;index<ruleStats.size();index++){
		if(ruleStats[index].size()==0)continue;
		auto& rs=ruleStats[index].back();
		double recTime=0;
		unsigned groundRule=0;
		for(auto& rs:ruleStats[index]){
			recTime+=rs.time;
			groundRule+=rs.groundedRule;
		}
		unsigned time=recTime/getTotalTime()*100;
 		string ss=appendSpace(to_string(time)+"%%",8)+appendSpace(ruleStats[index][0].rule,RULESPACE)+appendSpace(to_string(recTime),TIMESPACE)+appendSpace(to_string(groundRule),GROUNDSPACE)+to_string(ruleStats[index].size());

		vec.emplace_back(ss,true,0);
		unsigned fatherRule=vec.size();
		if(rs.rule.size()>=RULESPACE){
			for (unsigned i = 0; i < rs.rule.size(); i += RULESPACE*2)
				vec.emplace_back(rs.rule.substr(i, RULESPACE*2),false,fatherRule);
		}
		for(unsigned i=0;i<rs.bodyPES.size();i++){
			if(rs.bodyPES[i].first.size()==0)continue;
			double size=rs.bodyPES[i].second;
			string ss=appendSpace(rs.bodyPES[i].first,ATOMSPACE)+"P.E.S. = "+to_string(rs.bodyPES[i].second);
			vec.emplace_back(ss,false,fatherRule);
			unsigned fatherAtom=vec.size();
			for(auto& sel:rs.varSelectivity[i]){
				unsigned select=sel.second/size*100;
				string ss=sel.first+"  Selectivity = "+to_string(sel.second)+"("+to_string(select)+"%)";
				vec.emplace_back(ss,false,fatherAtom);
			}
		}

	}
	return vec;
}

RuleStatistics* RuleStatistics::getInstance() {
	if(rstats==nullptr)
		rstats=new RuleStatistics();
	return rstats;
}

void RuleStatistics::rawPrint(ostream& ss){
	string heading1="Total time: "+to_string(getTotalTime());
	string heading2="Time%   "+appendSpace("Rule",RULESPACE)+appendSpace("Time(s)",TIMESPACE)+appendSpace("N.GroundRule ",GROUNDSPACE)+appendSpace("Iteration",TIMESPACE)+"Join Eval.";
	ss<<heading1<<endl;
	ss<<heading2<<endl;
	auto tStats=Options::globalOptions()->getPrintGroundStats();
	for(unsigned i=0;i<ruleStats.size();i++){
		if(tStats==BASE_STATS)
			ss<<rawRuleStatNoSpaceCut(i);
		else if(tStats==ADV_STATS)
			ss<<rawRuleStat(i);
	}
}

string RuleStatistics::rawRuleStat(unsigned index){
	string ss="";
	if(ruleStats[index].size()==0)return "";
	auto& rs=ruleStats[index].back();
	double recTime=0;
	unsigned groundRule=0;
	for(auto& rs:ruleStats[index]){
		recTime+=rs.time;
		groundRule+=rs.groundedRule;
	}
	unsigned time=recTime/getTotalTime()*100;
	ss=appendSpace(to_string(time)+"%",8)+appendSpace(ruleStats[index][0].rule,RULESPACE)+appendSpace(to_string(recTime),TIMESPACE)+appendSpace(to_string(groundRule),GROUNDSPACE)+to_string(ruleStats[index].size())+'\n';
	if(ruleStats[index][0].rule.size()>=RULESPACE)
		ss+=ruleStats[index][0].rule+'\n';
	for(unsigned i=0;i<rs.bodyPES.size();i++){
		if(rs.bodyPES[i].first.size()==0)continue;
		double size=rs.bodyPES[i].second;
		ss+=appendSpace(rs.bodyPES[i].first,ATOMSPACE)+"SIZE = "+to_string(rs.bodyPES[i].second)+'\n';
		for(auto& sel:rs.varSelectivity[i]){
			unsigned select=sel.second/size*100;
			ss+=sel.first+"  Selectivity = "+to_string(sel.second)+"("+to_string(select)+"%)"+'\n';
		}
	}
	return ss;
}

string RuleStatistics::rawRuleStatNoSpaceCut(unsigned index){
	string ss="";
	if(ruleStats[index].size()==0)return "";
	double recTime=0;
	unsigned groundRule=0;
	for(auto& rs:ruleStats[index]){
		recTime+=rs.time;
		groundRule+=rs.groundedRule;
	}
	unsigned time=recTime/getTotalTime()*100;
	ss=appendSpace(to_string(time)+"%",8)+appendSpace(ruleStats[index][0].rule,ruleStats[index][0].rule.size()+8)+appendSpace(to_string(recTime),TIMESPACE)+appendSpace(to_string(groundRule),GROUNDSPACE)+appendSpace(to_string(ruleStats[index].size()),GROUNDSPACE)+to_string(ruleStats[index][0].joinEvaluation)+'\n';
	return ss;
}



void RuleStatistics::prepareStats(Rule* rule){
	unsigned index=rule->getIndex();
	if(ruleStats.size()<=index){
		ruleStats.resize(index+1);
	}

	vector<pair<string,unsigned>> bodyPES;
	vector<vector<pair<string,unsigned>>> varSelectivity;
	bodyPES.resize(rule->getSizeBody());
	varSelectivity.resize(rule->getSizeBody());
	for(unsigned i=0;i<rule->getSizeBody();i++){
		Atom* atom=rule->getAtomInBody(i);
		if(!atom->isClassicalLiteral() || atom->isNegative())continue;
		stringstream ss;
		atom->print(ss);
		unsigned size=PredicateExtTable::getInstance()->getPredicateExt(atom->getPredicate())->getPredicateExtentionSize();
		bodyPES[i]={ss.str(),size};
		vector<pair<string,unsigned>> select;
		for(unsigned j=0;j<atom->getTermsSize();j++){
			Term* t=atom->getTerm(j);
			if(t->getType()==VARIABLE){
				PredicateInformation* pi=PredicateExtTable::getInstance()->getPredicateExt(atom->getPredicate())->getPredicateInformation();
				if(pi!=nullptr){
					unsigned selectivity=pi->getSelectivity(j);
					stringstream ss1;
					t->print(ss1);
					select.push_back({ss1.str(),selectivity});
				}
			}
		}
		if(select.size()>0)
			varSelectivity[i]=select;
	}
	stringstream ss;
	rule->print(ss);
	string str=ss.str();
	str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
	ruleStats[index].emplace_back(str,0,bodyPES,varSelectivity,rcej.evaluate(rule));

}



} /* namespace grounder */
} /* namespace DLV2 */
