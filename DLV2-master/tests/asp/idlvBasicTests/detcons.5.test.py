input = """
% Here the GUS computation is needed.
% Roughly, the following should be done:
% mbt becomes must-be-true because of the constraint.
% f and f2's support counters are decremented.
% The GUS for f and f2's component is computed, deriving their falsity.
% t and t2 become true.

:- not mbt.
mbt | f.
mbt | f2.
f :- f2.
f2 :- f.

t :- not f.
t2 :- not f2.
"""
output = """
{mbt, t, t2}
"""
