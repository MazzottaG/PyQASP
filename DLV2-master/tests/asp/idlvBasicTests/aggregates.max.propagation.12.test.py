input = """
% This was constructed to uncover a bug in the aggregate propagation.

% a(1) should immediately be derived true by the model generator.
% Including a fact would be different, as the aggregate below would be
% simplified before the model generator starts.
a(1) | never1.
:- not a(1).

% a(2) should be initially undefined.
a(2) | na(2).

% a(3) should be initially must be true.
:- not a(3).
% these rules are to avoid a(3) becoming true immediately.
a(3) :- a(2).
a(3) :- na(2).

% With this setting (a(1) true, a(2) undefined, a(3) mbt) the following
% happened as of 2006-03-14: When a(3) became mbt, the aggregate was detected
% to be violated, hence false. On becoming false, it was then tried to ensure
% violation (if possible). Here the problem occurred: On checking whether
% the aggregate is already violated, only true (and no must-be-true) atoms
% are considered. Since in this case the aggregate is violated only by a
% must-be-true atom, it was considered unviolated. Moreover, since only one
% undefined atom exists, which is undefined and can violate the aggregate
% (a(2)), this is derived as must-be-true, which incorrectly inhibits the
% answer set in which a(2) is false.
never2 :- #max{ D: a(D) } = 1.
"""
output = """
{a(1), a(2), a(3)}
{a(1), a(3), na(2)}
"""
