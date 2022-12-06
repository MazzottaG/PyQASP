input = """
% a cell may live, except for the ones at the border
{ lives(X,Y) } <= 1 :- value(X), value(Y), X>0, Y>0, size(N), X<=N, Y<=N.
size(2).
value(0).
value(1).
value(2).
value(3).
"""
output = """
{size(2), value(0), value(1), value(2), value(3)}
{lives(2,1), size(2), value(0), value(1), value(2), value(3)}
{lives(1,2), size(2), value(0), value(1), value(2), value(3)}
{lives(1,2), lives(2,1), size(2), value(0), value(1), value(2), value(3)}
{lives(2,2), size(2), value(0), value(1), value(2), value(3)}
{lives(1,2), lives(2,2), size(2), value(0), value(1), value(2), value(3)}
{lives(2,1), lives(2,2), size(2), value(0), value(1), value(2), value(3)}
{lives(1,2), lives(2,1), lives(2,2), size(2), value(0), value(1), value(2), value(3)}
{lives(1,1), size(2), value(0), value(1), value(2), value(3)}
{lives(1,1), lives(2,1), size(2), value(0), value(1), value(2), value(3)}
{lives(1,1), lives(2,2), size(2), value(0), value(1), value(2), value(3)}
{lives(1,1), lives(2,1), lives(2,2), size(2), value(0), value(1), value(2), value(3)}
{lives(1,1), lives(1,2), size(2), value(0), value(1), value(2), value(3)}
{lives(1,1), lives(1,2), lives(2,2), size(2), value(0), value(1), value(2), value(3)}
{lives(1,1), lives(1,2), lives(2,1), size(2), value(0), value(1), value(2), value(3)}
{lives(1,1), lives(1,2), lives(2,1), lives(2,2), size(2), value(0), value(1), value(2), value(3)}
"""
