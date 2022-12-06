#ifndef UTILITY_H
#define UTILITY_H

#include <vector>
#include <string>
#include <sstream>
using namespace std;

class Utility {

    public:
        static void split(const string &s, char delim, vector< string >& output ) {
            stringstream ss( s );
            string item;
            while( getline( ss, item, delim ) )
                output.push_back( item );
        }

        static void trim_right(string& s) {
            while(s.size() != 0) {
                if(s.back() == ' ' || s.back()=='\t') s.pop_back();
                else return;
            }
        }

        static void trim_left(string& s) {
            string::iterator it = s.begin();
            while(it != s.end()) {
                if(s.front() == ' ' || s.front()=='\t') ++it;
                else break;
            }
            s.erase(s.begin(), it);
        }

        static void trim(string& s) {
            trim_left(s);
            trim_right(s);
        }


};

#endif
