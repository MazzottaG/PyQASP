input = """
% Propagation should proceed roughly as follows:
% mbt becomes must-be-true via the constraint.
% mbt has only one potentially supporting rule, so it should be derived as
% true and t should be derived as must be true aswell.
% t also has only one potentially supporting rule, so it should be derived as
% true and f should be derived as false.

:- not mbt.
mbt :- t.
f | t.

"""
output = """
{mbt, t}
"""
