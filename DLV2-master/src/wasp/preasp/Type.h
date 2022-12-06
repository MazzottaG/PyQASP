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

#ifndef TYPE_H
#define	TYPE_H

namespace preasp {

class Type {
    #define TYPE_POSITIVEBODY 0
    #define TYPE_NEGATIVEBODY 1
    #define TYPE_POSITIVEAGGREGATE 2
    #define TYPE_NEGATIVEAGGREGATE 3

    public:
        Type(unsigned int id, unsigned int type) : id_(id), type_(type) {}
        unsigned int id() const { return id_; }        
        unsigned int type() const { return type_; }
    
    private:
        unsigned int id_ : 30;
        unsigned int type_ : 2;
};

}

#endif
