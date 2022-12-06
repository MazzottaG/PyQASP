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


#include <iostream>
#include <algorithm>
#include <climits>
#include "Graph.h"
using namespace std;


namespace DLV2{
namespace grounder{



Graph::~Graph() {
	// TODO Auto-generated destructor stub
}


void Graph::removeVertex(unsigned v) {
	for(auto& bucket:adj_list){
		vector<node> toRemove;
		for(auto e:bucket){
			if(e.first==v)
				toRemove.push_back(e);
		}
		for(auto e:toRemove)
			bucket.erase(e);
	}
	adj_list[v].clear();
	vertices.erase(v);
}

Graph::edges_vector Graph::getEdges() const {
	Graph::edges_vector edges;
	unsigned i=0;
	for(auto it=adj_list.begin();it!=adj_list.end();it++,i++){
		for(auto e:(*it)){
			edges.push_back(make_tuple(i,e.first,e.second));
		}
	}
	return edges;
}

Graph::strongconn_vector Graph::tarjan() {
	strongconn_vector strong;
	if(vertices.size()==0)return strong;
	unsigned i=1;
	unsigned max=*(vertices.begin());
	for(auto v:vertices)
		if(max<v)max=v;

	vector<unsigned> index(max+1,0);
	vector<unsigned> lowlink(max+1,INT_MAX);
	vector<unsigned> stack;

	for(auto v:vertices){
		if(index[v]==0){
			strongconnectTarjan(v,i,index,lowlink,stack,strong);
		}
	}

	return strong;

}

void Graph::printDot(string graphName,stringstream& ss) {

	ss<<"digraph "<<graphName<<"{"<<endl;

	for(unsigned i=0;i<adj_list.size();i++){
		for(auto e:adj_list[i]){
			ss<<"\t"<<i<<"->"<<e.first<<" [label="<<e.second<<"];"<<endl;
		}
	}

	ss<<"}"<<endl;

}

bool Graph::haveIncomingEdges(unsigned vertex,const adjacency_list& adj_list){
	bool find=false;
	for(auto& bucket:adj_list){
		if(bucket.count({vertex,0})){
			find=true;
			break;
		}
	}
	return find;

}

vector<unsigned> Graph::kahn() {
	vector<unsigned> topSort;
	vector<unsigned> vertices;
	for(auto v:this->vertices){
		if(!haveIncomingEdges(v))
			vertices.push_back(v);
	}
	adjacency_list adj_list=this->adj_list;
	while(!vertices.empty()){
		unsigned n=vertices.back();
		vertices.pop_back();
		topSort.push_back(n);
		vector<unsigned> positive;
		vector<unsigned> negative;
		vector<node> toRemove;
		for(auto e:adj_list[n]){
			if(e.second>=0)
				positive.push_back(e.first);
			else
				negative.push_back(e.first);
			toRemove.push_back(e);
		}
		for(auto trm:toRemove){
			adj_list[n].erase(trm);
		}
		for(auto v:negative)
			if(!haveIncomingEdges(v,adj_list))
				vertices.push_back(v);
		for(auto v:positive)
			if(!haveIncomingEdges(v,adj_list))
				vertices.push_back(v);
	}

	for(auto& e:adj_list)
		if(e.size()>0)
			throw Graph::topological_exception();

	return topSort;
}

void Graph::strongconnectTarjan(unsigned v,unsigned& i, vector<unsigned>& index,
		vector<unsigned>& lowlink, vector<unsigned>& stack,strongconn_vector& strong) {

	index[v]=i;
	lowlink[v]=i;
	i++;
	stack.push_back(v);
	for(auto& e:adj_list[v]){
		unsigned w=e.first;
		if(index[w]==0){
			strongconnectTarjan(w,i,index,lowlink,stack,strong);
			lowlink[v]=min(lowlink[v],lowlink[w]);
		}else if(find(stack.begin(),stack.end(),w)!=stack.end()){
			lowlink[v]=min(lowlink[v],index[w]);
		}
	}

	if(lowlink[v]==index[v]){

		unsigned w;
		strong.emplace_back();
		do{
			w=stack.back();
			stack.pop_back();
			strong.back().push_back(w);
		}while(w!=v);
	}

}


}
}
