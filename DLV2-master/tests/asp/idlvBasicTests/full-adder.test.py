input = """
% The and-gates:

out(X,1) :- and(X), in1(X,1), in2(X,1), not ab(X).
out(X,0) :- and(X), in1(X,0), not ab(X).
out(X,0) :- and(X), in2(X,0), not ab(X).

% The or-gates:

out(X,0) :- or(X), in1(X,0), in2(X,0), not ab(X).
out(X,1) :- or(X), in1(X,1), not ab(X).
out(X,1) :- or(X), in2(X,1), not ab(X).

% The xor-gates:

out(X,1) :- xor(X), in1(X,1), in2(X,0), not ab(X).
out(X,1) :- xor(X), in1(X,0), in2(X,1), not ab(X).
out(X,0) :- xor(X), in1(X,Y), in2(X,Y), not ab(X).

% After these general definitions, we have to model the concrete
% circuit topology:

and(a1).
and(a2).
or(o1).
xor(x1).
xor(x2).

in1(a1,S) :- in1(x1,S).
in1(x1,S) :- in1(a1,S).

in2(a1,S) :- in2(x1,S).
in2(x1,S) :- in2(a1,S).

in2(x2,S) :- in1(a2,S).
in1(a2,S) :- in2(x2,S).

out(x1,S) :- in1(x2,S).
in1(x2,S) :- out(x1,S).

out(x1,S) :- in2(a2,S).
in2(a2,S) :- out(x1,S).

out(a1,S) :- in2(o1,S).
in2(o1,S) :- out(a1,S).

out(a2,S) :- in1(o1,S).
in1(o1,S) :- out(a2,S).

:- out(X,0), out(X,1).
:- in1(X,0), in1(X,1).
:- in2(X,0), in2(X,1).

gate(X) :- and(X).
gate(X) :- xor(X).
gate(X) :- or(X).

out(X,0) | out(X,1) :- gate(X).
in1(X,0) | in1(X,1) :- gate(X).
in2(X,0) | in2(X,1) :- gate(X).

"""
output = """
{and(a1), and(a2), gate(a1), gate(a2), gate(o1), gate(x1), gate(x2), in1(a1,1), in1(a2,1), in1(o1,0), in1(x1,1), in1(x2,0), in2(a1,1), in2(a2,0), in2(o1,1), in2(x1,1), in2(x2,1), or(o1), out(a1,1), out(a2,0), out(o1,1), out(x1,0), out(x2,1), xor(x1), xor(x2)}
{and(a1), and(a2), gate(a1), gate(a2), gate(o1), gate(x1), gate(x2), in1(a1,0), in1(a2,1), in1(o1,0), in1(x1,0), in1(x2,0), in2(a1,0), in2(a2,0), in2(o1,0), in2(x1,0), in2(x2,1), or(o1), out(a1,0), out(a2,0), out(o1,0), out(x1,0), out(x2,1), xor(x1), xor(x2)}
{and(a1), and(a2), gate(a1), gate(a2), gate(o1), gate(x1), gate(x2), in1(a1,1), in1(a2,0), in1(o1,0), in1(x1,1), in1(x2,0), in2(a1,1), in2(a2,0), in2(o1,1), in2(x1,1), in2(x2,0), or(o1), out(a1,1), out(a2,0), out(o1,1), out(x1,0), out(x2,0), xor(x1), xor(x2)}
{and(a1), and(a2), gate(a1), gate(a2), gate(o1), gate(x1), gate(x2), in1(a1,0), in1(a2,0), in1(o1,0), in1(x1,0), in1(x2,0), in2(a1,0), in2(a2,0), in2(o1,0), in2(x1,0), in2(x2,0), or(o1), out(a1,0), out(a2,0), out(o1,0), out(x1,0), out(x2,0), xor(x1), xor(x2)}
{and(a1), and(a2), gate(a1), gate(a2), gate(o1), gate(x1), gate(x2), in1(a1,1), in1(a2,1), in1(o1,1), in1(x1,1), in1(x2,1), in2(a1,0), in2(a2,1), in2(o1,0), in2(x1,0), in2(x2,1), or(o1), out(a1,0), out(a2,1), out(o1,1), out(x1,1), out(x2,0), xor(x1), xor(x2)}
{and(a1), and(a2), gate(a1), gate(a2), gate(o1), gate(x1), gate(x2), in1(a1,0), in1(a2,1), in1(o1,1), in1(x1,0), in1(x2,1), in2(a1,1), in2(a2,1), in2(o1,0), in2(x1,1), in2(x2,1), or(o1), out(a1,0), out(a2,1), out(o1,1), out(x1,1), out(x2,0), xor(x1), xor(x2)}
{and(a1), and(a2), gate(a1), gate(a2), gate(o1), gate(x1), gate(x2), in1(a1,1), in1(a2,0), in1(o1,0), in1(x1,1), in1(x2,1), in2(a1,0), in2(a2,1), in2(o1,0), in2(x1,0), in2(x2,0), or(o1), out(a1,0), out(a2,0), out(o1,0), out(x1,1), out(x2,1), xor(x1), xor(x2)}
{and(a1), and(a2), gate(a1), gate(a2), gate(o1), gate(x1), gate(x2), in1(a1,0), in1(a2,0), in1(o1,0), in1(x1,0), in1(x2,1), in2(a1,1), in2(a2,1), in2(o1,0), in2(x1,1), in2(x2,0), or(o1), out(a1,0), out(a2,0), out(o1,0), out(x1,1), out(x2,1), xor(x1), xor(x2)}
"""
