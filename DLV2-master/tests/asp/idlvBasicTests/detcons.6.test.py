input = """
% The following should be done:
% mbt becomes must-be-true because of the constraint.
% mbt | f. is the last potentially supporting rule for mbt, so it must be its
% supporting rule. Hence mbt becomes true and f becomes false.
% Susequently, t becomes true.

:- not mbt.
mbt | f.

t :- not f.
"""
output = """
{mbt, t}
"""
