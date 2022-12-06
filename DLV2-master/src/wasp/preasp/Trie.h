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

#ifndef TRIE_H
#define TRIE_H

#include <cassert>
#include <climits>
#include <iostream>
#include <unordered_map>
#include <vector>
using namespace std;

namespace preasp {

class Node {

    public:
        inline Node(int content_for_node) : content(content_for_node), label(UINT_MAX){}
        inline ~Node() { for( unordered_map<int, Node*>::iterator it = childrenMap.begin(); it != childrenMap.end(); ++it ) { assert( it->second ); delete it->second; } }

        inline int getContent() const { return content; }        
        inline bool hasLabel() const { return label != UINT_MAX; }
        inline unsigned int getLabel() const { return label; }
        inline void setLabel( unsigned int l ) { label = l; }

        Node* findChild(int c) {
            if(childrenMap.find(c) != childrenMap.end()) return childrenMap[ c ];
            return NULL;
        }

        inline void addChild(Node* child) { assert( childrenMap.find( child->getContent() ) == childrenMap.end() ); childrenMap[ child->getContent() ] = child; }

    private:
        int content;
        unsigned int label;
        unordered_map< int, Node* > childrenMap;
};

class Trie {
    public:
        Trie() : current(NULL), numberOfLabels(0), disabled(0) { root = new Node( UINT_MAX ); }
        ~Trie() { clear(); }
        
        inline void clear() { delete root; root = NULL; }
        inline void disable() { if(disabled) return; clear(); disabled = 1; }        
        inline void setNumberOfLabels(unsigned int nbOfLabels) { numberOfLabels = nbOfLabels; }
        inline void startInsertion() { if(!disabled) { assert(current == NULL); current = root; } }
        
        inline bool addElement(int element) {
            if(disabled) return false;
            assert(current != NULL);
            Node* node = current->findChild(element);
            if(node == NULL) { node = new Node(element); current->addChild(node); }
            assert(node != NULL);
            current = node;
            return current->hasLabel();
        }
        
        inline unsigned int getLabel() const { return current->getLabel(); }
        
        inline bool endInsertion() {
            if(disabled) return false;
            assert(current != NULL);    
            bool tmp = true;
            if(!current->hasLabel()) { current->setLabel( ++numberOfLabels ); tmp = false; }
            current = NULL;
            return tmp;
        }
        
        inline unsigned int endInsertionGetLabel() {
            if(disabled) return UINT_MAX;
            assert(current != NULL);    
            unsigned int label = current->getLabel();
            if(!current->hasLabel()) { current->setLabel( ++numberOfLabels ); }
            current = NULL;
            return label;
        }
        
        inline unsigned int endInsertionGetLabel(unsigned int newAux) {
            if(disabled) return UINT_MAX;
            assert(current != NULL);    
            unsigned int label = current->getLabel();
            if(!current->hasLabel()) { current->setLabel(newAux); }
            current = NULL;
            return label;
        }
        
    private:
        Node* root;
        Node* current;
        unsigned int numberOfLabels : 31;
        unsigned int disabled : 1;
        inline Trie(const Trie& orig);
};

}

#endif
