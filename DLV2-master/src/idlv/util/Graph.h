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
#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <unordered_set>
#include <sstream>
#include <algorithm>

using namespace std;

namespace DLV2{
namespace grounder{
/**
 *  This class represent a adjacency_list graph. The vertex of the graph must be unsigned int and the weight
 *  of the edge can be positive or negative integer
 */
class Graph {
public:
	///Value in the adjacent list of the graph: First element the id of the vertex, second the weight of the edge
	using node=pair<unsigned,int>;
	///Vector of edges. First and second element are the vertices and the third element are the weight
	using edges_vector = vector<tuple<unsigned,unsigned,int>>;
	///vector of strong component. Each bucket rappresent the strong component and the vertices belonging to it.s
	using strongconn_vector = vector<vector<unsigned>>;
	///Hash and Equal function for the node
	struct HashNode{
		bool operator()(const node& p1,const node& p2)const{
			return p1.first==p2.first;
		}
		size_t operator()(const node& p1)const{
			return p1.first;
		}
	};
	using adjacency_list = vector<unordered_set<node,HashNode,HashNode>>;
	///Exception for the topological sort algorithm
	class topological_exception:public exception{};


	Graph(bool updatePolicy=true):updateIfNewEdgeIsGreater(updatePolicy){};
	virtual ~Graph();

	///Add edge from node e1 to node e2 with the weight equal to value.
	///If exist already an edge from e1 to e2 and the weight then update accordine the updateIfNewEdgeIsGreater value
	void addEdge(unsigned e1,unsigned e2,int value){
		unsigned maxvalue=max(e1,e2);
		if(adj_list.size()<=maxvalue)adj_list.resize(maxvalue+1);
		auto it=adj_list[e1].insert({e2,value});
		if(!it.second){
			if(it.first->second!=value){
				int previousVal=it.first->second;
				adj_list[e1].erase(*(it.first));
				unsigned newValue=(updateIfNewEdgeIsGreater)?max(value,previousVal):min(value,previousVal);
				adj_list[e1].insert({e2,newValue});
			}
		}
		vertices.insert(e1);
		vertices.insert(e2);
	}

	void addVertex(unsigned v){
		if(adj_list.size()<=v)adj_list.resize(v+1);
		vertices.insert(v);
	}

	unordered_set<unsigned> getVertices()const{
		return vertices;
	}

	unsigned getNumVertices(){
		return vertices.size();
	}

	unsigned getNumEdges(){
		unsigned i=0,count=0;
		for(auto it=adj_list.begin();it!=adj_list.end();it++,i++){
			for(unsigned j=0;j<it->size();j++){
				count++;
			}
		}
		return count;
	}

	///Remove the vertex from the graph. Then remove all edge that involve the vertex.
	void removeVertex(unsigned v);

	//Remove the edge in the graph
	void removeEdge(unsigned v1,unsigned v2){
		if(adj_list.size()>v1 && adj_list[v1].size()>0){
			node n;
			n.first=v2;
			adj_list[v1].erase(n);
		}
	}

	///Return true if the vertex have an incoming edge in the adjacency_list adj_list
	static bool haveIncomingEdges(unsigned,const adjacency_list& adj_list);

	///Return true if the vertex have an incoming edge in the adjacency_list of the class
	bool haveIncomingEdges(unsigned vertex){
		return haveIncomingEdges(vertex,adj_list);
	}

	///Return all the edge of the graph
	edges_vector getEdges()const;

	///Calculate the strong component using the tarjan algorithm
	Graph::strongconn_vector tarjan();

	///Calculate the topological sort using the kahn algorithm. If The graph contains
	///cycle thrown an exception
	vector<unsigned> kahn();

	///Print the graph in the dot format
	void printDot(string,stringstream& s);

private:

	void strongconnectTarjan(unsigned v,unsigned& i,vector<unsigned>& index,vector<unsigned>& lowlink,vector<unsigned>& stack,strongconn_vector& strong);


private:
	/// Adjacency list : for each vertex in the graph exist a bucket in the vector. Each bucket contains a
	/// vector of node
	adjacency_list adj_list;
	///Set of vertices
	unordered_set<unsigned> vertices;
	/// If exist same edge with multiple weight, if true update with the greater weight otherwise with the lower weight.
	bool updateIfNewEdgeIsGreater;

};


template<typename T>
class UntypedGraph {
private:
	struct Vertex{
		Vertex(T name,unsigned i):name(name),index(i){};
		Vertex():name(),index(0){};
		bool operator()(const Vertex& v1,const Vertex& v2)const{
			return v1.name==v2.name;
		}
		size_t operator()(const Vertex& v)const{
			hash<T> hf;
			return hf(v.name);
		}
		T name;
		unsigned index;
	};
public:
	using strong_vec = vector<vector<T>>;
	using edge_vector = vector<tuple<T,T,int>>;

	UntypedGraph(bool updatePolicy=true):g(updatePolicy),index(1){};

	void addEdge(T v1,T v2,int weight){
		unsigned i1=addVertex(v1).index;
		unsigned i2=addVertex(v2).index;
		g.addEdge(i1,i2,weight);
	}

	void addAVertex(T v){
		unsigned i1=addVertex(v).index;
		g.addVertex(i1);
	}

	void removeEdge(T v1,T v2){
		unsigned i1=addVertex(v1).index;
		unsigned i2=addVertex(v2).index;
		g.removeEdge(i1,i2);

	}

	edge_vector getEdges(){
		edge_vector edgesuntyped;
		auto edges=g.getEdges();
		for(auto& e:edges)
			edgesuntyped.push_back(make_tuple(getName(get<0>(e)),getName(get<1>(e)),get<2>(e)));
		return edgesuntyped;
	}

	void printDot(string graphName,stringstream& ss){

		ss<<"digraph "<<graphName<<"{"<<"\n";

		for(auto tup:g.getEdges()){
			auto color=(get<2>(tup)>=0)?"blue":"red";
			ss<<"\t\""<<getName(get<0>(tup))<<"\"->\""<<getName(get<1>(tup))<<"\" [color="<<color<<"];"<<"\n";
		}

		ss<<"}"<<"\n";
	}

	unsigned getNumVertices(){
		return g.getNumVertices();
	}

	unsigned getNumEdges(){
		return g.getNumEdges();
	}

	strong_vec calculateStrongComponent(){
		Graph::strongconn_vector strong=g.tarjan();
		strong_vec svec;
		svec.reserve(strong.size());
		for(auto& c:strong){
			svec.emplace_back();
			for(auto e:c)
				svec.back().push_back(getName(e));
		}
		return svec;
	}

	vector<T> calculateTopologicalSort(){
		vector<unsigned> tp=g.kahn();
		vector<T> vec;
		vec.reserve(tp.size());
		for(auto i:tp)
			vec.push_back(getName(i));

		return vec;
	}





private:

	T getName(unsigned i){
		for(auto v:vertices)
			if(v.index==i)
				return v.name;
		return T();
	}

	Vertex addVertex(T v){
		Vertex ver(v,index);
		auto res =vertices.insert(ver);
		if(res.second)
			++index;
		else
			ver.index=(res.first->index);

		return ver;
	}


	Graph g;
	unsigned index;
	unordered_set<Vertex,Vertex,Vertex> vertices;
};

}
}

#endif /* GRAPH_H_ */
