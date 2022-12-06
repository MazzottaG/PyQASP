input = """
% Propagation should proceed roughly as follows:
% mbt1 and mbt2 become must-be-true via constraints.
% f1 becomes false because the head of its only potentially supporting rule
% contains mbt2 which is mbt and so it cannot support f1.
% Additionally, mbt2 has only one potentially supporting rule, so it should be
% derived as true.
% Then mbt1 has only a single supporting rule, as mbt1 :- f1 has a false body,
% and so mbt1 must become true via mbt1 :- mbt3, hence mbt3 must be true.
% mbt3 has only one potentially supporting rule, so it should be derived as
% true, and f2 should be derived as false.


:- not mbt2.
f1 | mbt2.

:- not mbt1.
mbt1 :- f1.
mbt1 :- mbt3.

f2 | mbt3.
"""
output = """
{mbt1, mbt2, mbt3}
"""
