input = """
% Similar to detcons.2, but somewhat simpler.
% The following should be done:
% mbt becomes must-be-true because of the constraint.
% mbt cannot be derived as true, as it still has two potentially supporting
% rules, both of which have a yet undefined body.
% f's potentially support counter should be decreased and becomes 0.
% f is then derived false.
% Susequently, t becomes true and mbt becomes supported by two rules.

:- not mbt.
mbt | f :- t.
mbt :- t.
t :- not f.

"""
output = """
{mbt, t}
"""
