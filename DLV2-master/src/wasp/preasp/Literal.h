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

#ifndef LITERAL_H
#define	LITERAL_H

namespace preasp {

class Literal {
    public:
        inline Literal() : var_(0), sign_(1) {}
        inline Literal(int lit) {
            if(lit > 0) {var_ = lit; sign_ = 0;}
            else {var_ = -lit; sign_ = 1; }
        }
        inline Literal(unsigned int var, bool sign) : var_(var), sign_(sign) {}
        inline Literal(const Literal& orig) : var_(orig.var_), sign_(orig.sign_) {}
                
        inline unsigned int var() const { return var_; }
        inline void setVar(unsigned int var) { var_ = var; }
        inline bool sign() const { return sign_; }
        inline int toInt() const { return sign_ ? -var_ : var_; }        
        inline bool operator<(const Literal& lit) const { return toInt() < lit.toInt(); }
        inline bool operator==(const Literal& lit) const { return toInt() == lit.toInt(); }
        inline bool operator!=(const Literal& lit) const { return toInt() != lit.toInt(); }
                
    private:
        unsigned int var_ : 31;
        unsigned int sign_ : 1;
};

inline Literal operator~(Literal lit) { return Literal(lit.var(), lit.sign() ? 0 : 1); }

}

#endif
