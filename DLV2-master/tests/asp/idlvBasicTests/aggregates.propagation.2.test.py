input = """
%%%%%%%%%%%%%%%%%%%%%%%
% The inference from constraints derives:
% MustBeTrue(#count)
% False(a).
% 
% False(a) -> True(b(1)).
%
% At this point, since MustBeTrue(#count), 
% performing a new propagation 
% False(b(2)) is derived,
% hence True(c).
%%%%%%%%%%%%%%%%%%%%%%%


:- not 1 = #count {X : b(X)}.
b(2) | c.
 
b(1) :- not a.
a    :- not b(1).
 
:- a.
"""
output = """
{b(1), c}
"""
