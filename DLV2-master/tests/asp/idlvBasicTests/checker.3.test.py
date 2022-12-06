input = """
% Example of non-HCF program
% By Cristinel Mateis.
% Originally known as "full-head-cycle". 

a | b.
c :- a.
c :- b.
d | e :- a.
d :- e.
e :- d, not b.

% solutions:   {b, c}
%              {a, c, d, e}
"""
output = """
{b, c}
{a, c, d, e}
"""
