/*
 *  Copyright (C) 2017 Carmine Dodaro (carminedodaro@gmail.com)
 *  Copyright (C) 2017 Mario Alviano (mario@alviano.net)
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

#include "DependencyGraph.h"

#include <climits>
#include <unordered_map>

namespace preasp {

unsigned int Component::nextId = 0;

inline static unsigned int VISIT(unsigned int v) { return (v << 2) | 0x0u; }
inline static unsigned int VISITEDGE(unsigned int v) { return (v << 2) | 0x1u; }
inline static unsigned int POSTVISIT(unsigned int v) { return (v << 2) | 0x2u; }

inline static unsigned int getNode(unsigned int x) { return x >> 2; }
inline static bool isVISIT(unsigned int x) { return (x & 0x3u) == 0x0u; }
inline static bool isVISITEDGE(unsigned int x) { return (x & 0x3u) == 0x1u; }
inline static bool isPOSTVISIT(unsigned int x) { return (x & 0x3u) == 0x2u; }

void DependencyGraph::computeSCC(bool allComponents) {
    clearGraph();
    bool* identified = new bool[graph.size()];
    vector<unsigned int> stack;
    unsigned int* index = new unsigned int[graph.size()];
    vector<unsigned int> boundaries;

    for(unsigned int i = 1; i < graph.size(); i++) {
        identified[i] = false;
        index[i] = UINT_MAX;
    }

    vector<unsigned int> todo;
    vector<unsigned int> component;
    scc.reserve(graph.size());

    for(unsigned int v = 1; v < graph.size(); v++) {
        if(index[v] != UINT_MAX) continue;
        todo.clear();
        todo.push_back(VISIT(v));
        do{
            unsigned int x = todo.back();
            todo.pop_back();
            unsigned int v = getNode(x);
            if(isVISIT(x)) {
                index[v] = stack.size();
                stack.push_back(v);
                boundaries.push_back(index[v]);
                todo.push_back(POSTVISIT(v));
                const vector<EdgeTo>& edges = graph[v];
                for(unsigned int i = 0; i < edges.size(); i++) todo.push_back(VISITEDGE(edges[i].dest()));
            }
            else if(isVISITEDGE(x)) {
                if(index[v] == UINT_MAX) todo.push_back(VISIT(v));
                else if(!identified[v]) while(index[v] < boundaries.back()) boundaries.pop_back();
            }
            else {
                assert(isPOSTVISIT(x));
                if(boundaries.back() == index[v]) {
                    boundaries.pop_back();
                    component.clear();
                    for(unsigned int i = index[v]; i < stack.size(); i++) {
                        if(identified[stack[i]]) continue;
                        identified[stack[i]] = true;
                        component.push_back(stack[i]);
                    }
                    assert(component.size() > 0);
                    if(component.size() > 1) {
                        for(unsigned int i = 0; i < component.size(); i++) setCyclicComponent(component[i], scc.size());
                        scc.push_back(new Component(component));
                    }
                    else if(allComponents) {
                        scc.push_back(new Component(component));
                    }
                    stack.resize(index[v]);
                }

            }
        } while(!todo.empty());
    }

    delete[] index;
    delete[] identified;
}

bool DependencyGraph::hasOddCycle(unsigned int component) const {
    const vector<unsigned int>& atoms_ = atoms(component);
    unordered_map<unsigned int, bool> atomsToCheck;
    for(unsigned int i = 0; i < atoms_.size(); i++) {
        unsigned int v = atoms_[i];
        if(atomsToCheck.find(v) == atomsToCheck.end()) atomsToCheck[v] = true;
        bool status = atomsToCheck[v];
        const vector<EdgeTo>& edges = graph[v];
        for(unsigned int j = 0; j < edges.size(); j++) {
            unsigned int dest = edges[j].dest();
            if(v == dest || sameComponent(v,dest)) {
                bool isPos = edges[j].isPositive();
                if(atomsToCheck.find(dest) == atomsToCheck.end())
                    atomsToCheck[dest] = isPos ? status : !status;
                else if((isPos && atomsToCheck[dest] != status) || (!isPos && atomsToCheck[dest] == status))
                    return true;
            }
        }
    }

    return false;
}

}
