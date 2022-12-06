input = """
int(1..10).
a(X) | not_a(X) :- int(X).
ok :- 0 < #count{X:a(X)}< 2.
:- not ok.
"""
output = """
{a(1), int(1), int(10), int(2), int(3), int(4), int(5), int(6), int(7), int(8), int(9), not_a(10), not_a(2), not_a(3), not_a(4), not_a(5), not_a(6), not_a(7), not_a(8), not_a(9), ok}
{a(2), int(1), int(10), int(2), int(3), int(4), int(5), int(6), int(7), int(8), int(9), not_a(1), not_a(10), not_a(3), not_a(4), not_a(5), not_a(6), not_a(7), not_a(8), not_a(9), ok}
{a(3), int(1), int(10), int(2), int(3), int(4), int(5), int(6), int(7), int(8), int(9), not_a(1), not_a(10), not_a(2), not_a(4), not_a(5), not_a(6), not_a(7), not_a(8), not_a(9), ok}
{a(4), int(1), int(10), int(2), int(3), int(4), int(5), int(6), int(7), int(8), int(9), not_a(1), not_a(10), not_a(2), not_a(3), not_a(5), not_a(6), not_a(7), not_a(8), not_a(9), ok}
{a(5), int(1), int(10), int(2), int(3), int(4), int(5), int(6), int(7), int(8), int(9), not_a(1), not_a(10), not_a(2), not_a(3), not_a(4), not_a(6), not_a(7), not_a(8), not_a(9), ok}
{a(6), int(1), int(10), int(2), int(3), int(4), int(5), int(6), int(7), int(8), int(9), not_a(1), not_a(10), not_a(2), not_a(3), not_a(4), not_a(5), not_a(7), not_a(8), not_a(9), ok}
{a(7), int(1), int(10), int(2), int(3), int(4), int(5), int(6), int(7), int(8), int(9), not_a(1), not_a(10), not_a(2), not_a(3), not_a(4), not_a(5), not_a(6), not_a(8), not_a(9), ok}
{a(8), int(1), int(10), int(2), int(3), int(4), int(5), int(6), int(7), int(8), int(9), not_a(1), not_a(10), not_a(2), not_a(3), not_a(4), not_a(5), not_a(6), not_a(7), not_a(9), ok}
{a(10), int(1), int(10), int(2), int(3), int(4), int(5), int(6), int(7), int(8), int(9), not_a(1), not_a(2), not_a(3), not_a(4), not_a(5), not_a(6), not_a(7), not_a(8), not_a(9), ok}
{a(9), int(1), int(10), int(2), int(3), int(4), int(5), int(6), int(7), int(8), int(9), not_a(1), not_a(10), not_a(2), not_a(3), not_a(4), not_a(5), not_a(6), not_a(7), not_a(8), ok}
"""
