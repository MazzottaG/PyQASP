input = """
%        +
%     a /
%      + PMCok - not b - +
%   b /               e /
%    + PMCf
% c /
%   +
% d |
%  MCf

a | useless1.
b | useless2 :- a.

c | useless3 :- b.

d | useless4 :- b.

e | useless4 :- not b.

:- not b, not f.

g :- e.

f | e | g.
f | stupid :- f.

:- b, not v.
uf1 :- v, b.
uf2 :- v, b.
uf3 :- v, b.
uf1 | uf3 | uf2 :- b.
v :- uf1, b.
v :- uf2, b.

"""
output = """
{a, f, useless2, useless4}
{f, useless1, useless4}
"""
