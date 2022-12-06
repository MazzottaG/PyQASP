input = """
% 3 pegs, n disks (given with option -N)
peg(p1).
peg(p2).
peg(p3).
location(X) :- peg(X).
location(X) :- .% #int(X).
smaller(D,D1) :- D<D1. %#int(D), #int(D1), D<D1.
smaller(D,P) :- peg(P). %#int(D), peg(P).
"""
output = """
"""
