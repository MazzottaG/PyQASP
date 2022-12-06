input = """
% Reported by Marcello Balduccini <marcello.balduccini@ttu.edu> on 2006-09-18.

h(a,0).
g(c).
g(a).


% rule 1
h(c,1) :- o(2).

% rule 2
goal_f(0) :- g(Lit), not h(Lit,0).

% rule 3
goal_h(0) :- not goal_f(0).

% rule 4
o(1) | k :- not goal_h(0).

"""
output = """
{g(a), g(c), goal_f(0), h(a,0), o(1)}
{g(a), g(c), goal_f(0), h(a,0), k}
"""
