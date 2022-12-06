input = """
% This test-case is conceived to verify that the normalization of the auxiliary atom name,
% internally generated in correspondence to the aggregate atom conjunction, is done correctly.
%
% Name normalization, consists in:
% 1) creation of a new name of the form "pred"index; index starts from 0 and it is incremented 
%    each time the string "pred"index already occurs in the program as predicate name;
% 2) substitution of the old internal name with the new one.
% 
%        ...
%        char str[21];
%        pair<index_t,bool> res;
%        do
%            {
%            sprintf(str,"pred%u",MaxInternallyDefined);
%            res = substitute(str, getArity(index), getType(index), index);
%            MaxInternallyDefined++;
%            }
%        while(! res.second);
%
% Here, pred0 and pred1 already occour in the program, so the auxiliary atom name as to be
% renamed as "pred"index with index>1. 
 
 
pred0(1) | pred1(1).
:- pred0(X).
a(1) | a(2).
b(2,1).
f(1).
d :- f(Z), #count{X:a(X),b(X,Y)}= Z.
"""
output = """
{a(1), b(2,1), f(1), pred1(1)}
{a(2), b(2,1), d, f(1), pred1(1)}
"""
