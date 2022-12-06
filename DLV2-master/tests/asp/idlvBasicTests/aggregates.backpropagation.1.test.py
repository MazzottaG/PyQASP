input = """
% This has been engineered to trigger the same bug as found with bug.86.
% To this end we need an aggregate atom that is derived as must-be-true
% and becomes itself true in the same propagation step and before propagation
% of the must-be-true aggregate.

% A choice.
z | y.
% Either choice entails x.
x :- z.
x :- y.

% When the truth of x is propagated, derive truth of a(2) and put it into the
% propagation queue.
a(2) :- x.

% Then, each of the following aggregates will be derived as must-be-true and
% put into the propagation queue.
% Then a(2) will be taken from the queue, and each of the following aggragate
% atoms will be derived as true.
% Finally the aggregates are taken from the queue, and they are no more
% must-be-true but have turned true in the meantime, which (wrongly) triggered
% an assertion.
:- x, not #count{X: a(X)} > 0.
:- x, not #sum{X: a(X)} > 0.
:- x, not #min{X: a(X)} > 0.
:- x, not #max{X: a(X)} > 0.
"""
output = """
{a(2), x, z}
{a(2), x, y}
"""
