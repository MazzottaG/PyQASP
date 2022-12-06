input = """
% This was constructed to uncover a bug in the aggregate propagation.

% a(3) should immediately be derived true by the model generator.
% Including a fact would be different, as the aggregate below would be
% simplified before the model generator starts.
a(3) | never1.
:- not a(3).

% a(2) should be initially undefined.
a(2) | na(2).

% a(1) should be initially must be true.
:- not a(1).
% these rules are to avoid a(1) becoming true immediately.
a(1) :- a(2).
a(1) :- na(2).

% With this setting (a(1) mbt, a(2) undefined, a(3) true) in an old
% inactive implementation the mbt atom was neither counted as true nor
% undefined, wrongly assuming that a(2) being true is the only way of
% violating the aggregate, which has just been derived false. It was
% falsely derived must-be-true because of this, missing an answer set
% (in which a(2) is false).
never2 :- #min{ D: a(D) } = 3.
"""
output = """
{a(1), a(2), a(3)}
{a(1), a(3), na(2)}
"""
