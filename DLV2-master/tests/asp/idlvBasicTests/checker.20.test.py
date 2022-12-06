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
b | useless2.

c | useless3 :- b.

d | useless4 :- b.

e | useless5 :- not b.

:- not mbt1, e.
mbt2 :- mbt1.

:- not mbt3, e.
mbt4 :- mbt3.

mbt1 | mbt2 | mbt3 :- mbt4.
mbt2 | mbt3 | mbt4 :- mbt1.
mbt3 | mbt4 | mbt1 :- mbt2.


:- b, not v.
uf1 :- v.
uf2 :- v.
uf3 :- v.
uf1 | uf3 | uf2.
v :- uf1.
"""
output = """
{uf2, useless1, useless2, useless5}
{a, uf2, useless2, useless5}
{a, uf3, useless2, useless5}
{uf3, useless1, useless2, useless5}
"""
