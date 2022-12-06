input = """
ntables(2).    % number of tables
nchairs(1).    % number of chairs
npersons(2).  % number of people


table(1).
table(2).
person(1).
person(2).

at(P,T) | not_at(P,T) :- person(P), table(T).

:- not X=#count { P: at(P,T) }, table(T), nchairs(X).

:- not #count{T : at(P,T)} = 1, person(P). 

:- not at(1,1).  
"""
output = """
{at(1,1), at(2,2), nchairs(1), not_at(1,2), not_at(2,1), npersons(2), ntables(2), person(1), person(2), table(1), table(2)}
"""
