/*
 *  Copyright (C) 2017  Carmine Dodaro (carminedodaro@gmail.com)
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

#ifndef DEPENDENCYGRAPH_H
#define	DEPENDENCYGRAPH_H

#include <vector>
#include <cassert>
#include <iostream>
#include <climits>
using namespace std;

namespace preasp {

class Component
{
    public:
        inline Component(vector<unsigned int>& atoms) : id_(INT_MAX), hcf_(true) { atoms_.swap(atoms); }
        inline void setNonHcf() {
            hcf_ = false;
            if(nextId < id_) id_ = nextId++;
        }
        inline bool hcf() const { return hcf_; }
        inline unsigned int id() const { return id_; }
        inline const vector<unsigned int>& atoms() const { return atoms_; }

    private:
        unsigned int id_ : 31;
        unsigned int hcf_ : 1;
        vector<unsigned int> atoms_;
        static unsigned int nextId;
};

class EdgeTo
{
    public:
        inline EdgeTo(unsigned int dest, bool pos) : dest_(dest) { pos_ = pos ? 1 : 0; }
        inline EdgeTo(unsigned int dest) : dest_(dest), pos_(1) {}
        inline bool isNegative() const { return !pos_; }
        inline bool isPositive() const { return pos_; }
        inline unsigned int dest() const { return dest_; }

    private:
        unsigned int dest_ : 31;
        unsigned int pos_ : 1;
};

class DependencyGraph
{
    public:
        DependencyGraph() : hcf_(true) {}
        ~DependencyGraph() { deepClean(); }
        inline void init(unsigned int numberOfVars) { graph.reserve(numberOfVars); }
        inline void deepClean() {
            for(unsigned int i = 0; i < scc.size(); i++) { delete scc[i]; }
            vector<Component*> tmp; scc.swap(tmp);
            vector<vector<EdgeTo> > tmp2; graph.swap(tmp2);
            vector<unsigned int> tmp3; atomToCyclicComponent.swap(tmp3);
        }

        inline void addEdge(unsigned int v1, unsigned int v2) {
            addVertex(v1);    addVertex(v2);
            assert(containsVertex(v1) && containsVertex(v2));
            if(getComponent(v1)==0) return;
            graph[v1].push_back(EdgeTo(v2));
        }

        inline void addEdge(unsigned int v1, unsigned int v2, bool positive) {
            addVertex(v1);    addVertex(v2);
            assert(containsVertex(v1) && containsVertex(v2));
            if(getComponent(v1)==0) return;
            graph[v1].push_back(EdgeTo(v2, positive));
        }

        inline void removeAllEdges(unsigned int v) {
            addVertex(v);
            assert(containsVertex(v));
            graph[v].clear();
            setCyclicComponent(v, 0);
        }

        void computeSCC() { computeSCC(false); }
        void computeSCC(bool allComponents);
        inline unsigned int numberOfCyclicComponents() const { return scc.size(); }
        inline bool sameComponent(unsigned int at1, unsigned int at2) const {
            return isInCyclicComponent(at1) && isInCyclicComponent(at2) && getComponent(at1) == getComponent(at2);
        }

        inline bool isInCyclicComponent(unsigned int atom) const {
            return atom < atomToCyclicComponent.size() && atomToCyclicComponent[atom] != UINT_MAX; }

        inline unsigned int getCyclicComponent(unsigned int atom) const {
            assert(isInCyclicComponent(atom));
            assert(atom < atomToCyclicComponent.size()); return atomToCyclicComponent[atom];
        }

        inline void setNonHcf(unsigned int component) {
            assert(component < scc.size());
            scc[component]->setNonHcf();
            hcf_ = false;
        }

        inline bool hcf(unsigned int component) const {
            assert(component < scc.size());
            return scc[component]->hcf();
        }

        inline unsigned int id(unsigned int component) const {
            assert(component < scc.size());
            return scc[component]->id();
        }

        inline bool hcf() const {
            return hcf_;
        }

        inline const vector<unsigned int>& atoms(unsigned int component) const { return scc[component]->atoms(); }

        bool hasOddCycle(unsigned int component) const;
        DependencyGraph(const DependencyGraph& orig) = delete;
        DependencyGraph& operator=(const DependencyGraph& orig) = delete;

    private:        
        vector<Component*> scc;
    	vector<vector<EdgeTo> > graph;
        vector<unsigned int> atomToCyclicComponent;
        bool hcf_;

        inline bool containsVertex(unsigned int v) const { return v < graph.size(); }
        inline void addVertex(unsigned int v) {
            while(!containsVertex(v)) {
                graph.push_back(vector<EdgeTo>());
                atomToCyclicComponent.push_back(UINT_MAX);
            }
        }
        inline unsigned int getComponent(unsigned int atom) const { return atom < atomToCyclicComponent.size() ? atomToCyclicComponent[atom] : UINT_MAX; }
        inline void setCyclicComponent(unsigned int atom, unsigned int component) { assert(atom < atomToCyclicComponent.size()); atomToCyclicComponent[atom] = component; }
        inline void clearGraph() {
            for(unsigned int i = 0; i < atomToCyclicComponent.size(); i++)
                if(atomToCyclicComponent[i] == 0) {
                    atomToCyclicComponent[i] = UINT_MAX;
                    graph[i].clear();
                }
        }
};

}

#endif
